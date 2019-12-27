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

	/**
	 * @class LogAppenderIf
	 *
	 * @brief Interface to print message in the logger
	 * @sa Logger
	 */
	class LogAppenderIf {
	public:
		using UPtr = std::unique_ptr<LogAppenderIf>;
		virtual ~LogAppenderIf() = default;

		virtual void print(LogLevel logLevel, const std::string& msg) = 0;
	};

	/**
	 * @class ConsoleAppender
	 *
	 * @brief Implements out log messages to std::cout
	 */
	class ConsoleAppender : public LogAppenderIf {
	public:
		using UPtr = std::unique_ptr<LogAppenderIf>;
		void print(LogLevel logLevel, const std::string& msg) override;
	};


	/**
	 * @class
	 *
	 * @brief Implement logging
	 */
	class Logger {
	public:
		/**
		 * @brief Sets the lowest log level for all log messages
		 * @note to set off all logs use LogLevel::OFF
		 * @param level
		 */
		static void setLogLevel(LogLevel level);

		/**
		 * @brief Sets appender to print messages for all log messages
		 *
		 * The default appender is ConsoleAppender
		 * @param appender
		 */
		static void setAppender(LogAppenderIf::UPtr appender);

		Logger(LogLevel level);

		/**
		 * @brief Add message to the log
		 * @tparam T type of the data to print
		 * @param msg The message to print
		 * @return
		 */
		template <typename T>
		std::ostringstream& operator << (T msg) {
			_stream << msg;
			return _stream;
		}

		/**
		 * @brief Default destructor
		 *
		 * The destructor prints all messages, that were added by << operator, before the logger are destroyed
		 */
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
