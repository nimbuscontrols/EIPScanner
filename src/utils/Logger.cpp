//
// Created by Aleksey Timin on 11/16/19.
//
#include <iostream>
#include <map>

#include "Logger.h"

namespace eipScanner {
namespace utils {
	LogLevel Logger::_globalLogLevel = LogLevel::INFO;

	Logger::Logger(LogLevel logLevel): _logLevel{logLevel} {

	}

	Logger::~Logger() {
		std::map<LogLevel, std::string> logNames = {
			std::make_pair(LogLevel::TRACE, "[TRACE] "),
			std::make_pair(LogLevel::DEBUG, "[DEBUG] "),
			std::make_pair(LogLevel::INFO, "[INFO] "),
			std::make_pair(LogLevel::WARNING, "[WARNING] "),
			std::make_pair(LogLevel::ERROR, "[ERROR] "),
		};

		if (_logLevel <= _globalLogLevel) {
			std::cout << logNames[_logLevel] << _stream.str() << std::endl;
		}
	}

	void Logger::setLogLevel(LogLevel level) {
		_globalLogLevel = level;
	}

}
}