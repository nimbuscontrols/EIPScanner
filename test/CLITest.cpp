//
// Created by Aleksey Timin on 11/16/19.
//

#include <cstdlib>
#include "SessionInfo.h"
#include "MessageRouter.h"
#include "utils/Logger.h"

using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::cip::ServiceCodes;
using eipScanner::cip::GeneralStatusCodes;
using eipScanner::cip::EPath;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

int main() {
	Logger::setLogLevel(LogLevel::DEBUG);
	auto si = std::make_shared<SessionInfo>("127.0.0.1", 0xAF12);

	MessageRouter messageRouter;

	auto response = messageRouter.sendRequest(si,
			ServiceCodes::GET_ATTRIBUTE_SINGLE,
			EPath(0x04, 100, 3),
			std::vector<uint8_t>());

	if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
		Logger(LogLevel::INFO) << "We did it!";
	} else {
		Logger(LogLevel::ERROR) << "We got error=" << response.getGeneralStatusCode();
	}
	exit(0);
}