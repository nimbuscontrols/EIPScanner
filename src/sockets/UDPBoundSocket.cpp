//
// Created by Aleksey Timin on 11/21/19.
//
#include <system_error>
#include <sys/socket.h>
#include <netinet/in.h>
#include "UDPBoundSocket.h"

namespace eipScanner {
namespace sockets {


	UDPBoundSocket::UDPBoundSocket(std::string host, int port)
		: UDPBoundSocket(EndPoint(host, port)) {

	}
	UDPBoundSocket::UDPBoundSocket(EndPoint endPoint)
		: UDPSocket(std::move(endPoint)) {
		int on = 1;
		if (setsockopt(_sockedFd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)) < 0) {
			throw std::system_error(errno, std::generic_category());
		}

		auto addr = _remoteEndPoint.getAddr();
		addr.sin_addr.s_addr = INADDR_ANY;
		if (bind(_sockedFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			throw std::system_error(errno, std::generic_category());
		}
	}

	sockets::UDPBoundSocket::~UDPBoundSocket() = default;
}
}