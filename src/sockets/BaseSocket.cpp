//
// Created by Aleksey Timin on 11/18/19.
//

#include <sys/socket.h>
#include <sys/select.h>
#include <utility>
#include <algorithm>
#include <system_error>
#include "BaseSocket.h"

namespace eipScanner {
namespace sockets {
	BaseSocket::BaseSocket(std::string host, int port)
			: _host{std::move(host)}
			, _port{port}
			, _recvTimeout(0)
			, _beginReceiveHandler() {
	}

	BaseSocket::~BaseSocket() = default;

	int BaseSocket::getSocketFd() const {
		return _sockedFd;
	}

	const std::string &BaseSocket::getHost() const {
		return _host;
	}

	int BaseSocket::getPort() const {
		return _port;
	}

	const std::chrono::milliseconds &BaseSocket::getRecvTimeout() const {
		return _recvTimeout;
	}

	void BaseSocket::setRecvTimeout(const std::chrono::milliseconds &recvTimeout) {
		_recvTimeout = recvTimeout;

		struct timeval tv = {

#ifdef __APPLE__
		.tv_sec = static_cast<__darwin_suseconds_t>(recvTimeout.count()/1000),
				.tv_usec =  static_cast<__darwin_suseconds_t>((recvTimeout.count()%1000)*1000)
#elif __linux__
				.tv_sec = static_cast<__time_t>(recvTimeout.count()/1000),
				.tv_usec =  static_cast<__time_t>((recvTimeout.count()%1000)*1000)

#elif _WIN32
		// not sure what the macro is for windows
				.tv_sec = static_cast<_time64>(recvTimeout.count()/1000),
				.tv_usec =  static_cast<_time64>((recvTimeout.count()%1000)*1000)
#endif

		};
		setsockopt(_sockedFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	}

	void BaseSocket::setBeginReceiveHandler(BaseSocket::BeginReceiveHandler handler) {
		_beginReceiveHandler = std::move(handler);
	}

	void BaseSocket::BeginReceive() {
		_beginReceiveHandler(*this);
	}

	void BaseSocket::select(std::vector<BaseSocket::SPtr> sockets, std::chrono::milliseconds timeout) {
		// Receive data
		struct timeval tv = {
				.tv_sec = static_cast<__time_t>(timeout.count()/1000),
				.tv_usec =  static_cast<__time_t>((timeout.count()%1000)*1000)
		};

		fd_set recvSet;
		FD_ZERO(&recvSet);
		for (auto& sock : sockets) {
			FD_SET(sock->getSocketFd(), &recvSet);
		}

		BaseSocket::SPtr socketWithMaxFd = *std::max_element(sockets.begin(), sockets.end(), [&recvSet](auto sock1, auto sock2) {
			return sock1->getSocketFd() < sock2->getSocketFd();
		});

		int ready;
		do {
			ready = ::select(socketWithMaxFd->getSocketFd() + 1, &recvSet, NULL, NULL, &tv);
			if (ready < 0) {
				throw std::system_error(errno, std::generic_category());
			}

			for (auto& sock : sockets) {
				if (FD_ISSET(sock->getSocketFd(), &recvSet)) {
					sock->BeginReceive();
					continue;
				}
			}
		} while(ready > 0);
	}

}
}