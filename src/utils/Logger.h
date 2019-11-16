//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_UTILS_LOGGER_H
#define EIPSCANNER_UTILS_LOGGER_H

#include <sstream>

namespace eipScanner {
namespace utils {

	enum LogLevel {
		ERROR = 0,
		WARNING,
		INFO,
		DEBUG,
		TRACE
	};

	class Logger {
	public:
		static void logLevel(LogLevel level);

		Logger(LogLevel level);

		template <typename T>
		std::ostringstream& operator << (T msg) {
			_stream << msg;
			return _stream;
		}

		~Logger();

	private:
		static LogLevel _globalLogLevel;

		LogLevel _logLevel;
		std::ostringstream _stream;
	};
}
}

#endif //EIPSCANNER_LOGGER_H
