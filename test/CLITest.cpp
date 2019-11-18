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
	auto si = std::make_shared<SessionInfo>("192.168.0.55", 0xAF12);
	auto messageRouter = std::make_shared<MessageRouter>(si);

	// Read attribute
	auto response = messageRouter->sendRequest(ServiceCodes::GET_ATTRIBUTE_SINGLE,
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


	response = messageRouter->sendRequest(ServiceCodes::SET_ATTRIBUTE_SINGLE,
						 EPath(0x04, 151, 3),
						 assembly151.data());

	if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
		Logger(LogLevel::INFO) << "Writing is successful";
	} else {
		Logger(LogLevel::ERROR) << "We got error=0x" << std::hex << response.getGeneralStatusCode();
	}

	// Implicit messaging
	ConnectionManager connectionManager(messageRouter);

	ConnectionParameters parameters;
	parameters.connectionPath = {0x20, 0x04, 0x24, 0x97, 0x2C, 0x96, 0x2C, 0x64};  // config Assm151, output Assm150, intput Assm100
	parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::P2P;
	parameters.t2oNetworkConnectionParams |= 32;
	parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::P2P;
	parameters.o2tNetworkConnectionParams |= 36; //TODO size of Assm150 = 32 but we need +4 I forgot why it is so!
	parameters.o2tRPI = 200000;
	parameters.t2oRPI = 200000;
	parameters.transportTypeTrigger |= NetworkConnectionParams::TRANSP_SERVER; // Enable watchdog in adapter

	auto io = connectionManager.forwardOpen(parameters);
	io.lock()->setReceiveDataListener([](auto data) {
		std::ostringstream ss;
		for (auto& byte : data) {
			ss << "[" << std::hex << (int)byte << "]";
		}

		Logger(LogLevel::INFO) << "Received: " << ss.str();
	});

	while (true) {
		connectionManager.handleConnections(std::chrono::milliseconds(100));
	}

	return 0;
}