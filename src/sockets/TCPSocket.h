//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_SOCKETS_TCPSOCKET_H
#define EIPSCANNER_SOCKETS_TCPSOCKET_H

#include <chrono>
#include <vector>

#include "BaseSocket.h"

namespace eipScanner {
namespace sockets {
	class TCPSocket : public BaseSocket {
	public:
		explicit TCPSocket(EndPoint endPoint);
		TCPSocket(EndPoint endPoint, std::chrono::milliseconds connTimeout);
		TCPSocket(std::string host, int port);
		virtual ~TCPSocket();

		void Send(const std::vector<uint8_t>& data) const override;
		std::vector<uint8_t> Receive(size_t size) const override;

	private:
	};
}
}

#endif  // EIPSCANNER_TCPSOCKET_H
