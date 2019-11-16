//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_SOCKETS_TCPSOCKET_H
#define EIPSCANNER_SOCKETS_TCPSOCKET_H

#include <vector>

namespace sockets {

	class TCPSocket {
	public:
		TCPSocket(std::string host, int port, size_t bufferSize);
		~TCPSocket();

		void Send(const std::vector<uint8_t>& data) const;
		std::vector<uint8_t> Receive(size_t size);

		int getSockedFd() const;
		const std::string &getHost() const;
		int getPort() const;

	private:
		int _sockedFd;
		std::string _host;
		int _port;
		std::vector<uint8_t> _recvBuffer;
	};
}


#endif  // EIPSCANNER_TCPSOCKET_H
