//
// Created by Aleksey Timin on 11/16/19.
//
#include "Logger.h"
#include <iostream>

namespace eipScanner {
namespace utils {
	LogLevel Logger::_globalLogLevel = LogLevel::INFO;

	Logger::Logger(LogLevel logLevel): _logLevel{logLevel} {

	}

	Logger::~Logger() {
		if (_logLevel <= _globalLogLevel) {
			std::cout << _stream.str() << std::endl;
		}
	}

	void Logger::setLogLevel(LogLevel level) {
		_globalLogLevel = level;
	}

}
}