//
// Created by Aleksey Timin on 11/18/19.
//

#ifdef __linux__
#include <sys/socket.h>
#include <sys/select.h>
#elif defined _WIN32
#include <winsock2.h>
#include <time.h>
#endif

#include <utility>
#include <algorithm>
#include <system_error>
#include "BaseSocket.h"

namespace eipScanner {
namespace sockets {

	BaseSocket::BaseSocket(EndPoint endPoint)
			: _remoteEndPoint(std::move(endPoint))
			, _recvTimeout(0)
			, _beginReceiveHandler() {

	}

	BaseSocket::BaseSocket(std::string host, int port)
			: BaseSocket(EndPoint(std::move(host), port)) {
	}

	BaseSocket::~BaseSocket() = default;

	int BaseSocket::getSocketFd() const {
		return _sockedFd;
	}

	const std::chrono::milliseconds &BaseSocket::getRecvTimeout() const {
		return _recvTimeout;
	}

	void BaseSocket::setRecvTimeout(const std::chrono::milliseconds &recvTimeout) {
		_recvTimeout = recvTimeout;

		timeval tv = makePortableInterval(recvTimeout);
		setsockopt(_sockedFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	}

	timeval BaseSocket::makePortableInterval(const std::chrono::milliseconds &recvTimeout) {
		struct timeval tv = {

#ifdef __APPLE__
		.tv_sec = static_cast<__darwin_suseconds_t>(recvTimeout.count()/1000),
		.tv_usec =  static_cast<__darwin_suseconds_t>((recvTimeout.count()%1000)*1000)
#elif __linux__
		.tv_sec = static_cast<__time_t>(recvTimeout.count()/1000),
		.tv_usec =  static_cast<__time_t>((recvTimeout.count()%1000)*1000)

#elif _WIN32
		// not sure what the macro is for windows
		.tv_sec = static_cast<long int>(recvTimeout.count()/1000),
		.tv_usec =  static_cast<long int>((recvTimeout.count()%1000)*1000)
#endif

		};

		tv.tv_sec = std::max<decltype(tv.tv_sec)>(tv.tv_sec, 0);
        tv.tv_usec = std::max<decltype(tv.tv_usec)>(tv.tv_usec, 0);
		return tv;
	}

	void BaseSocket::setBeginReceiveHandler(BaseSocket::BeginReceiveHandler handler) {
		_beginReceiveHandler = std::move(handler);
	}

	void BaseSocket::BeginReceive() {
		_beginReceiveHandler(*this);
	}

	void BaseSocket::select(std::vector<BaseSocket::SPtr> sockets, std::chrono::milliseconds timeout) {
		BaseSocket::SPtr socketWithMaxFd = *std::max_element(sockets.begin(), sockets.end(), [](auto sock1, auto sock2) {
			return sock1->getSocketFd() < sock2->getSocketFd();
		});

		auto startTime = std::chrono::steady_clock::now();
		auto stopTime = startTime + timeout;
		int ready;
		do {
			timeval tv = makePortableInterval(std::chrono::duration_cast<std::chrono::milliseconds>(stopTime-startTime));

			fd_set recvSet;
			FD_ZERO(&recvSet);
			for (auto& sock : sockets) {
				FD_SET(sock->getSocketFd(), &recvSet);
			}

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

			startTime = std::chrono::steady_clock::now();
		} while(ready > 0);
	}

	const EndPoint &BaseSocket::getRemoteEndPoint() const {
		return _remoteEndPoint;
	}

}
}
