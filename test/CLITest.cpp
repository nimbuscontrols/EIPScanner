//
// Created by Aleksey Timin on 11/16/19.
//

#include <cstdlib>
#include "SessionInfo.h"
#include "utils/Logger.h"

using eipScanner::SessionInfo;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

int main() {
	Logger::setLogLevel(LogLevel::DEBUG);
	SessionInfo("127.0.0.1", 0xAF12);
	exit(0);
}