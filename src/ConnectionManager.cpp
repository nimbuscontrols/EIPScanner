//
// Created by Aleksey Timin on 11/18/19.
//

#include "ConnectionManager.h"
#include "cip/connectionManager/ForwardOpenRequest.h"
#include "cip/connectionManager/ForwardOpenRequest.h"
#include "cip/connectionManager/NetworkConnectionParams.h"
#include "utils/Logger.h"

namespace eipScanner {
	using namespace cip::connectionManager;
	using namespace utils;
	using cip::MessageRouterResponse;
	using cip::EPath;
	using cip::GeneralStatusCodes;

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

		ForwardOpenRequest request(connectionParameters);
		auto response = _messageRouter->sendRequest(
				static_cast<cip::CipUsint>(ConnectionManagerServiceCodes::FORWARD_OPEN),
				EPath(6, 1), request.pack());

		IOConnection::WPtr ioConnection;
		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {

		} else {
			Logger(LogLevel::ERROR) << "Failed to establish connection with error="
				<< std::hex << response.getGeneralStatusCode();
		}

		return ioConnection;
	}
}