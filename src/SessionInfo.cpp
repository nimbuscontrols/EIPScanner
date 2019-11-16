//
// Created by Aleksey Timin  on 11/16/19.
//

#include "SessionInfo.h"
#include "utils/Logger.h"
#include <system_error>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace eipScanner {
	using utils::Logger;
	using utils::LogLevel;

	SessionInfo::SessionInfo(const std::string &host, int port) {
		Logger(LogLevel::DEBUG) << "Open socket";

		_sockedFd = socket(AF_INET, SOCK_STREAM, 0);
		if (_sockedFd < 0) {
			throw std::system_error(errno, std::generic_category());
		}

		Logger(LogLevel::DEBUG) << "Resolving host";
		struct sockaddr_in addr{};
		if (inet_aton(host.c_str(), &addr.sin_addr) < 0) {
			close(_sockedFd);
			throw std::system_error(errno, std::generic_category());
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		Logger(LogLevel::DEBUG) << "Connecting";
		if (connect(_sockedFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			close(_sockedFd);
			throw std::system_error(errno, std::generic_category());
		}
	}

	SessionInfo::~SessionInfo() {
		Logger(LogLevel::DEBUG) << "Close socket";
		shutdown(_sockedFd, SHUT_RDWR);
		close(_sockedFd);
	}

	void SessionInfo::sendAndReceive() const {

	}
}