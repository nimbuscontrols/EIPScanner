//
// Created by Aleksey Timin on 11/18/19.
//
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <random>

#include "ConnectionManager.h"
#include "eip/CommonPacket.h"
#include "cip/connectionManager/ForwardOpenRequest.h"
#include "cip/connectionManager/ForwardCloseRequest.h"
#include "cip/connectionManager/ForwardOpenResponse.h"
#include "cip/connectionManager/NetworkConnectionParams.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

namespace eipScanner {
	using namespace cip::connectionManager;
	using namespace utils;
	using cip::MessageRouterResponse;
	using cip::EPath;
	using cip::GeneralStatusCodes;
	using eip::CommonPacket;
	using sockets::UDPSocket;
	using sockets::UDPBoundSocket;
	using sockets::BaseSocket;

	ConnectionManager::ConnectionManager(MessageRouter::SPtr messageRouter)
		: _messageRouter(std::move(messageRouter))
		, _connectionMap()
		, _lastHandleTime(std::chrono::steady_clock::now()){

		std::random_device rd;
		std::uniform_int_distribution<cip::CipUint> dist(0, std::numeric_limits<cip::CipUint>::max());
		_incarnationId = dist(rd);
	}

	ConnectionManager::~ConnectionManager() = default;

	IOConnection::WPtr
	ConnectionManager::forwardOpen(SessionInfoIf::SPtr si, ConnectionParameters connectionParameters) {
		static int serialNumberCount = 0;
		connectionParameters.connectionSerialNumber = ++serialNumberCount;

		if ((connectionParameters.o2tNetworkConnectionParams
			& NetworkConnectionParams::MULTICAST) > 0) {
			static cip::CipUint idCount = _incarnationId << 16;
			connectionParameters.o2tNetworkConnectionId = ++idCount;
		}

		if ((connectionParameters.t2oNetworkConnectionParams
			 & NetworkConnectionParams::P2P) > 0) {
			static cip::CipUdint idCount = _incarnationId << 16;
			connectionParameters.t2oNetworkConnectionId = ++idCount;
		}

		connectionParameters.connectionPathSize =  (connectionParameters.connectionPath .size() / 2)
				+ (connectionParameters.connectionPath .size() % 2);

		if ((connectionParameters.transportTypeTrigger & NetworkConnectionParams::CLASS1) > 0
			|| (connectionParameters.transportTypeTrigger & NetworkConnectionParams::CLASS3) > 0) {
			connectionParameters.o2tNetworkConnectionParams += 2;
			connectionParameters.t2oNetworkConnectionParams += 2;
		}

		if (connectionParameters.o2tRealTimeFormat) {
			connectionParameters.o2tNetworkConnectionParams += 4;
		}

		if (connectionParameters.t2oRealTimeFormat) {
			connectionParameters.t2oNetworkConnectionParams += 4;
		}

		Buffer buffer;
		buffer << sockets::EndPoint("0.0.0.0", 2222);
		eip::CommonPacketItem addrItem(eip::CommonPacketItemIds::T2O_SOCKADDR_INFO, buffer.data());

		ForwardOpenRequest request(connectionParameters);
		auto messageRouterResponse = _messageRouter->sendRequest(si,
				static_cast<cip::CipUsint>(ConnectionManagerServiceCodes::FORWARD_OPEN),
				EPath(6, 1), request.pack(), {addrItem});

		IOConnection::SPtr ioConnection;
		if (messageRouterResponse.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			ForwardOpenResponse response;
			response.expand(messageRouterResponse.getData());

			Logger(LogLevel::INFO) << "Open IO connection O2T_ID=" << response.getO2TNetworkConnectionId()
				<< " T2O_ID=" << response.getT2ONetworkConnectionId()
				<< " SerialNumber " << response.getConnectionSerialNumber();

			ioConnection.reset(new IOConnection());
			ioConnection->_o2tNetworkConnectionId = response.getO2TNetworkConnectionId();
			ioConnection->_t2oNetworkConnectionId = response.getT2ONetworkConnectionId();
			ioConnection->_o2tAPI = response.getO2TApi();
			ioConnection->_t2oAPI = response.getT2OApi();
			ioConnection->_connectionTimeoutMultiplier = 4 << connectionParameters.connectionTimeoutMultiplier;
			ioConnection->_serialNumber = response.getConnectionSerialNumber();
			ioConnection->_transportTypeTrigger = connectionParameters.transportTypeTrigger;
			ioConnection->_o2tRealTimeFormat = connectionParameters.o2tRealTimeFormat;
			ioConnection->_t2oRealTimeFormat = connectionParameters.t2oRealTimeFormat;
			ioConnection->_connectionPath = connectionParameters.connectionPath;
			ioConnection->_originatorVendorId = connectionParameters.originatorVendorId;
			ioConnection->_originatorSerialNumber = connectionParameters.originatorSerialNumber;

			const eip::CommonPacketItem::Vec &additionalItems = messageRouterResponse.getAdditionalPacketItems();
			auto o2tSockAddrInfo = std::find_if(additionalItems.begin(), additionalItems.end(),
					[](auto item) { return item.getTypeId() == eip::CommonPacketItemIds::O2T_SOCKADDR_INFO; });

			if (o2tSockAddrInfo != additionalItems.end()) {
				Buffer sockAddrBuffer(o2tSockAddrInfo->getData());
				sockets::EndPoint endPoint("",0);
				sockAddrBuffer >> endPoint;

				if (endPoint.getHost() == "0.0.0.0") {
					ioConnection->_socket = std::make_unique<UDPSocket>(
							si->getRemoteEndPoint().getHost(), endPoint.getPort());
				} else {
					ioConnection->_socket = std::make_unique<UDPSocket>(endPoint);
				}
				
			} else {
				ioConnection->_socket = std::make_unique<UDPSocket>(si->getRemoteEndPoint().getHost(), 2222);	
			}

			findOrCreateSocket(sockets::EndPoint(si->getRemoteEndPoint().getHost(), 2222));

			auto result = _connectionMap
					.insert(std::make_pair(response.getT2ONetworkConnectionId(), ioConnection));
			if (!result.second) {
				Logger(LogLevel::ERROR)
					<< "Connection with T2O_ID=" << response.getT2ONetworkConnectionId()
					<< " already exists.";
			}
		} else {
			logGeneralAndAdditionalStatus(messageRouterResponse);
		}

		return ioConnection;
	}

	void ConnectionManager::forwardClose(SessionInfoIf::SPtr si, const IOConnection::WPtr& ioConnection) {
		if (auto ptr = ioConnection.lock()) {
			ForwardCloseRequest request;

			request.setConnectionPath(ptr->_connectionPath);
			request.setConnectionSerialNumber(ptr->_serialNumber);
			request.setOriginatorVendorId(ptr->_originatorVendorId);
			request.setOriginatorSerialNumber(ptr->_originatorSerialNumber);

			Logger(LogLevel::INFO) << "Close connection connection T2O_ID="
					<< ptr->_t2oNetworkConnectionId;

			auto messageRouterResponse = _messageRouter->sendRequest(si,
					static_cast<cip::CipUsint>(ConnectionManagerServiceCodes::FORWARD_CLOSE),
					EPath(6, 1), request.pack());

			if (messageRouterResponse.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
				Logger(LogLevel::WARNING)
					<< "Failed to close the connection in target with error=0x"
					<< std::hex
					<< messageRouterResponse.getGeneralStatusCode()
					<< ". But the connection is removed from ConnectionManager anyway";
			}

			auto rc = _connectionMap.erase(ptr->_t2oNetworkConnectionId);
			assert(rc);
		} else {
			Logger(LogLevel::WARNING) << "Attempt to close an already closed connection";
		}
	}

	void ConnectionManager::handleConnections(std::chrono::milliseconds timeout) {
		std::vector<BaseSocket::SPtr > sockets;
		std::transform(_socketMap.begin(), _socketMap.end(), std::back_inserter(sockets), [](auto entry) {
			auto fd = entry.second->getSocketFd();
			return entry.second;
		});

		BaseSocket::select(sockets, timeout);

		auto now = std::chrono::steady_clock::now();
		std::vector<cip::CipUdint> connectionsToClose;
		auto sinceLastHandle =
				std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastHandleTime);
		Logger(LogLevel::TRACE) << "Last call was " << sinceLastHandle.count() << "ms ago";
		for (auto& entry : _connectionMap) {
			if (!entry.second->notifyTick(sinceLastHandle)) {
				connectionsToClose.push_back(entry.first);
			}
		}

		for (auto& id : connectionsToClose) {
			_connectionMap.erase(id);
		}

		_lastHandleTime = now;
	}

	UDPBoundSocket::SPtr ConnectionManager::findOrCreateSocket(const sockets::EndPoint& endPoint) {
		auto socket = _socketMap.find(endPoint);
		if (socket == _socketMap.end()) {
			auto newSocket = std::make_shared<UDPBoundSocket>(endPoint);
			_socketMap[endPoint] = newSocket;
			newSocket->setBeginReceiveHandler([](sockets::BaseSocket& sock) {
				Logger(LogLevel::DEBUG) << "Received something";
			});

			newSocket->setBeginReceiveHandler([this](BaseSocket& sock) {
				auto recvData = sock.Receive(504);
				CommonPacket commonPacket;
				commonPacket.expand(recvData);

				// TODO: Check TypeIDs and sequnce of the packages
				Buffer buffer(commonPacket.getItems().at(0).getData());
				cip::CipUdint connectionId;
				buffer >> connectionId;
				Logger(LogLevel::DEBUG) << "Received data from connection T2O_ID=" << connectionId;

				auto io = _connectionMap.find(connectionId);
				if (io != _connectionMap.end()) {
					io->second->notifyReceiveData(commonPacket.getItems().at(1).getData());
				} else {
					Logger(LogLevel::ERROR) << "Received data from unknow connection T2O_ID=" << connectionId;
				}
			});

			return newSocket;
		}

		return socket->second;
	}

	bool ConnectionManager::hasOpenConnections() const {
		return !_connectionMap.empty();
	}

}