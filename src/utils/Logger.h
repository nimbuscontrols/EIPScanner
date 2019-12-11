//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_UTILS_LOGGER_H
#define EIPSCANNER_UTILS_LOGGER_H

#include <sstream>
#include <memory>

namespace eipScanner {
namespace utils {

	enum class LogLevel {
		OFF = 0,
		ERROR,
		WARNING,
		INFO,
		DEBUG,
		TRACE
	};

	class LogAppenderIf {
	public:
		using UPtr = std::unique_ptr<LogAppenderIf>;
		virtual ~LogAppenderIf() = default;

		virtual void print(const std::string& msg) = 0;
	};

	class ConsoleAppender : public LogAppenderIf {
	public:
		using UPtr = std::unique_ptr<LogAppenderIf>;
		void print(const std::string& msg) override;
	};


	class Logger {
	public:
		static void setLogLevel(LogLevel level);
		static void setAppender(LogAppenderIf::UPtr appender);

		Logger(LogLevel level);

		template <typename T>
		std::ostringstream& operator << (T msg) {
			_stream << msg;
			return _stream;
		}

		~Logger();

	private:
		static LogLevel _globalLogLevel;
		static LogAppenderIf::UPtr _appender;

		LogLevel _logLevel;
		std::ostringstream _stream;
	};
}
}

#endif //EIPSCANNER_LOGGER_H
