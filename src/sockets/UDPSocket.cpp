//
// Created by Aleksey Timin on 11/18/19.
//

#include <system_error>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "utils/Logger.h"
#include "UDPSocket.h"

namespace eipScanner {
namespace sockets {
	using eipScanner::utils::Logger;
	using eipScanner::utils::LogLevel;

	UDPSocket::UDPSocket(std::string host, int port)
			: BaseSocket{host, port} {

		_sockedFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (_sockedFd < 0) {
			throw std::system_error(errno, std::generic_category());
		}

		Logger(LogLevel::DEBUG) << "Opened socket fd=" << _sockedFd;

		Logger(LogLevel::DEBUG) << "Parsing IP from " << _host;

		if (inet_aton(_host.c_str(), &_addr.sin_addr) < 0) {
			close(_sockedFd);
			throw std::system_error(errno, std::generic_category());
		}

		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(_port);
	}

	UDPSocket::~UDPSocket() {
		Logger(LogLevel::DEBUG) << "Close socket fd=" << _sockedFd;
		shutdown(_sockedFd, SHUT_RDWR);
		close(_sockedFd);
	}

	void UDPSocket::Send(const std::vector <uint8_t> &data) const {
		Logger(LogLevel::TRACE) << "Send " << data.size() << " bytes from TCP socket #" << _sockedFd << ".";

		int count = sendto(_sockedFd, data.data(), data.size(), 0,
				(struct sockaddr *)&_addr, sizeof(_addr));
		if (count < data.size()) {
			throw std::system_error(errno, std::generic_category());
		}
	}

	std::vector<uint8_t> UDPSocket::Receive(size_t size) {
		std::vector<uint8_t> recvBuffer(size);

		auto len = recvfrom(_sockedFd, recvBuffer.data(), recvBuffer.size(), 0, NULL, NULL);
		if (len < 0) {
			throw std::system_error(errno, std::generic_category());
		}

		return recvBuffer;
	}
}
}