//
// Created by Aleksey Timin on 11/18/19.
//
#include <algorithm>


#include "ConnectionManager.h"
#include "eip/CommonPacket.h"
#include "cip/connectionManager/ForwardOpenRequest.h"
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
	using sockets::BaseSocket;

	ConnectionManager::ConnectionManager(MessageRouter::SPtr messageRouter)
		: _messageRouter(messageRouter)
		, _connectionMap() {
	}

	ConnectionManager::~ConnectionManager() = default;

	IOConnection::WPtr
	ConnectionManager::forwardOpen(ConnectionParameters connectionParameters) {
		connectionParameters.connectionSerialNumber = 10;
		if ((connectionParameters.o2tNetworkConnectionParams
			& NetworkConnectionParams::MULTICAST) > 0) {
			static cip::CipUdint idCount = 0;
			connectionParameters.o2tNetworkConnectionId = ++idCount;
		}

		if ((connectionParameters.t2oNetworkConnectionParams
			 & NetworkConnectionParams::P2P) > 0) {
			static cip::CipUdint idCount = 0;
			connectionParameters.t2oNetworkConnectionId = ++idCount;
		}

		connectionParameters.connectionPathSize =  (connectionParameters.connectionPath .size() / 2)
				+ (connectionParameters.connectionPath .size() % 2);

		ForwardOpenRequest request(connectionParameters);
		auto messageRouterResponse = _messageRouter->sendRequest(
				static_cast<cip::CipUsint>(ConnectionManagerServiceCodes::FORWARD_OPEN),
				EPath(6, 1), request.pack());

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

			ioConnection->_socket = findOrCreateSocket(_messageRouter->getSessionInfo()->getHost(), 2222);

			//TODO: Need checking if the connection already exists
			_connectionMap.insert(std::make_pair(response.getT2ONetworkConnectionId(), ioConnection));
		} else {
			Logger logger(LogLevel::ERROR);
			logger << "Failed to establish connection with error="
				   << std::hex << messageRouterResponse.getGeneralStatusCode()
				   << " additional statuses ";
			for (auto& additionalStatus : messageRouterResponse.getAdditionalStatus()) {
				logger << "[0x"
						<< additionalStatus
						<< "]";
			}
		}

		return ioConnection;
	}


	void ConnectionManager::handleConnections(std::chrono::milliseconds timeout) {
		std::vector<BaseSocket::SPtr > sockets;
		std::transform(_socketMap.begin(), _socketMap.end(), std::back_inserter(sockets), [](auto entry) {
			auto fd = entry.second->getSocketFd();
			return entry.second;
		});

		BaseSocket::select(sockets, timeout);
	}

	UDPSocket::SPtr ConnectionManager::findOrCreateSocket(const std::string& host, int port) {
		SocketKey socketKey = {
				.host = host,
				.port = port
			};

		auto socket = _socketMap.find(socketKey);
		if (socket == _socketMap.end()) {
			auto newSocket = std::make_shared<UDPSocket>(host, port);
			_socketMap[socketKey] = newSocket;
			newSocket->setBeginReceiveHandler([](sockets::BaseSocket& sock) {
				Logger(LogLevel::DEBUG) << "Received something";
			});

			newSocket->setBeginReceiveHandler([this](BaseSocket& sock) {
				auto recvData = sock.Receive(504);
				CommonPacket commonPacket;
				commonPacket.expand(recvData);

				// TODO: Check TypeIDs and sequnce of the packages
				Buffer buffer(commonPacket[0].getData());
				cip::CipUdint connectionId;
				buffer >> connectionId;
				Logger(LogLevel::DEBUG) << "Received data from connection ID=" << connectionId;

				auto io = _connectionMap[connectionId];
				if (io) {
					io->NotifyReceiveData(commonPacket[1].getData());
				} else {
					Logger(LogLevel::ERROR) << "Received data from unknow connection ID=" << connectionId;
				}
			});

			return newSocket;
		}

		return socket->second;
	}
}