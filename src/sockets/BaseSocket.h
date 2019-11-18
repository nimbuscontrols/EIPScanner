//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_SOCKETS_BASESOCKET_H
#define EIPSCANNER_SOCKETS_BASESOCKET_H

#include <vector>
#include <cstdint>
#include <string>
#include <chrono>

namespace eipScanner {
namespace sockets {
	class BaseSocket {
	public:
		BaseSocket(std::string host, int port, size_t bufferSize);
		virtual ~BaseSocket();

		virtual void Send(const std::vector<uint8_t>& data) const = 0;
		virtual std::vector<uint8_t> Receive(size_t size) = 0;

		const std::chrono::milliseconds &getRecvTimeout() const;
		void setRecvTimeout(const std::chrono::milliseconds &recvTimeout);

		int getSockedFd() const;
		const std::string &getHost() const;
		int getPort() const;

	protected:
		int _sockedFd;
		std::string _host;
		int _port;
		std::vector<uint8_t> _recvBuffer;
		std::chrono::milliseconds _recvTimeout;
	};
}
}


#endif  // EIPSCANNER_SOCKETS_BASESOCKET_H
