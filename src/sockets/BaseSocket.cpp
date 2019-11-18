//
// Created by Aleksey Timin on 11/18/19.
//

#include <sys/socket.h>
#include "BaseSocket.h"
namespace eipScanner {
namespace sockets {
	BaseSocket::BaseSocket(std::string host, int port, size_t bufferSize)
			: _host{std::move(host)}
			, _port{port}
			, _recvBuffer(bufferSize)
			, _recvTimeout(0){
	}

	BaseSocket::~BaseSocket() = default;

	int BaseSocket::getSockedFd() const {
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

}
}