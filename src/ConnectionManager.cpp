//
// Created by Aleksey Timin on 11/18/19.
//

#include "ConnectionManager.h"
#include "cip/connectionManager/ForwardOpenRequest.h"
#include "cip/connectionManager/ForwardOpenResponse.h"
#include "cip/connectionManager/NetworkConnectionParams.h"
#include "utils/Logger.h"

namespace eipScanner {
	using namespace cip::connectionManager;
	using namespace utils;
	using cip::MessageRouterResponse;
	using cip::EPath;
	using cip::GeneralStatusCodes;
	using sockets::UDPSocket;

	ConnectionManager::ConnectionManager(MessageRouter::SPtr messageRouter)
		: _messageRouter(messageRouter)
		, _connectionMap() {
	}

	ConnectionManager::~ConnectionManager() = default;

	IOConnection::WPtr
	ConnectionManager::forwardOpen(ConnectionParameters connectionParameters) {
		if ((connectionParameters.o2tNetworkConnectionParams
			& NetworkConnectionParams::MULTICAST) > 0) {
			static cip::CipUdint idCount = 0;
			connectionParameters.o2tNetworkConnectionId = idCount;
		}

		if ((connectionParameters.t2oNetworkConnectionParams
			 & NetworkConnectionParams::P2P) > 0) {
			static cip::CipUdint idCount = 0;
			connectionParameters.t2oNetworkConnectionId = idCount;
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

			ioConnection.reset(new IOConnection());
			ioConnection->_o2tNetworkConnectionId = response.getO2TNetworkConnectionId();
			ioConnection->_t2oNetworkConnectionId = response.getT2ONetworkConnectionId();
			ioConnection->_o2tAPI = response.getO2TApi();
			ioConnection->_t2oAPI = response.getT2OApi();
			ioConnection->_connectionTimeoutMultiplier = 4 << connectionParameters.connectionTimeoutMultiplier;

			ioConnection->_socket = findOrCreateSocket(_messageRouter->getSessionInfo()->getHost(), 2222);

			IOConnectionKey connectionKey = {
					.o2tNetworkId = ioConnection->_o2tNetworkConnectionId,
					.t2oNetworkId = ioConnection->_t2oNetworkConnectionId
			};

			//TODO: Need checking if the connection already exists
			_connectionMap.insert(std::make_pair(connectionKey, ioConnection));
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

	UDPSocket::SPtr ConnectionManager::findOrCreateSocket(const std::string& host, int port) {
		SocketKey socketKey = {
				.host = host,
				.port = port
			};

		auto socket = _socketMap.find(socketKey);
		if (socket == _socketMap.end()) {
			auto newSocket = std::make_shared<UDPSocket>(host, port, 504);
			_socketMap[socketKey] = newSocket;

			return newSocket;
		}

		return socket->second;
	}
}