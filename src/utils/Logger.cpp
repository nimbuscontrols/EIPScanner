//
// Created by Aleksey Timin on 11/16/19.
//
#include <iostream>
#include <map>

#include "Logger.h"

namespace eipScanner {
namespace utils {
	LogLevel Logger::_globalLogLevel = LogLevel::INFO;
	LogAppenderIf::UPtr Logger::_appender = std::make_unique<ConsoleAppender>();

	Logger::Logger(LogLevel logLevel): _logLevel{logLevel} {

	}

	Logger::~Logger() {
		if (_globalLogLevel == LogLevel::OFF) {
			return;
		}

		std::map<LogLevel, std::string> logNames = {
			std::make_pair(LogLevel::TRACE, "[TRACE] "),
			std::make_pair(LogLevel::DEBUG, "[DEBUG] "),
			std::make_pair(LogLevel::INFO, "[INFO] "),
			std::make_pair(LogLevel::WARNING, "[WARNING] "),
			std::make_pair(LogLevel::ERROR, "[ERROR] "),
		};

		if (_logLevel <= _globalLogLevel) {
			_appender->print(logNames[_logLevel] + (_stream).str());
		}
	}

	void Logger::setLogLevel(LogLevel level) {
		_globalLogLevel = level;
	}

	void Logger::setAppender(LogAppenderIf::UPtr appender) {
		_appender = std::move(appender);
	}

	void ConsoleAppender::print(const std::string &msg) {
		std::cout << msg << std::endl;
	}
}
}