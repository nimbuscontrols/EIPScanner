//
// Created by Aleksey Timin on 11/16/19.
//

#include <cstdlib>
#include <sstream>
#include <cip/connectionManager/NetworkConnectionParams.h>
#include "SessionInfo.h"
#include "MessageRouter.h"
#include "ConnectionManager.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::ConnectionManager;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;
using eipScanner::utils::Buffer;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

int main() {
	Logger::setLogLevel(LogLevel::DEBUG);
	auto si = std::make_shared<SessionInfo>("127.0.0.1", 0xAF12, std::chrono::seconds(10));
	auto messageRouter = std::make_shared<MessageRouter>();

	// Read attribute
	auto response = messageRouter->sendRequest(si, ServiceCodes::GET_ATTRIBUTE_SINGLE,
											   EPath(0x01, 1, 1),
											   {});

	if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
		Buffer buffer(response.getData());
		CipUint vendorId;
		buffer >> vendorId;

		Logger(LogLevel::INFO) << "Vendor ID is " << vendorId;
	} else {
		Logger(LogLevel::ERROR) << "We got error=0x" << std::hex << response.getGeneralStatusCode();
	}

	//Write attribute
	// See OpenEr EDS 160 line
	Buffer assembly151;
	assembly151 << CipUsint(1)
				<< CipUsint(2)
				<< CipUsint(3)
				<< CipUsint(4)
				<< CipUsint(5)
				<< CipUsint(6)
				<< CipUsint(7)
				<< CipUsint(8)
				<< CipUsint(9)
				<< CipUsint(10);


	response = messageRouter->sendRequest(si, ServiceCodes::SET_ATTRIBUTE_SINGLE,
										  EPath(0x04, 151, 3),
										  assembly151.data());

	if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
		Logger(LogLevel::INFO) << "Writing is successful";
	} else {
		Logger(LogLevel::ERROR) << "We got error=0x" << std::hex << response.getGeneralStatusCode();
	}


	return 0;
}