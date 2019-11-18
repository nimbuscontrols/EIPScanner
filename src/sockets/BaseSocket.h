//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_SOCKETS_BASESOCKET_H
#define EIPSCANNER_SOCKETS_BASESOCKET_H

#include <vector>
#include <cstdint>
#include <string>
#include <chrono>
#include <functional>
#include <memory>

namespace eipScanner {
namespace sockets {
	class BaseSocket {
	public:
		using BeginReceiveHandler = std::function<void(BaseSocket&)>;
		using SPtr = std::shared_ptr<BaseSocket>;

		BaseSocket(std::string host, int port);
		virtual ~BaseSocket();

		virtual void Send(const std::vector<uint8_t>& data) const = 0;
		virtual std::vector<uint8_t> Receive(size_t size) = 0;
		void setBeginReceiveHandler(BeginReceiveHandler handler);


		const std::chrono::milliseconds &getRecvTimeout() const;
		void setRecvTimeout(const std::chrono::milliseconds &recvTimeout);

		int getSocketFd() const;
		const std::string &getHost() const;
		int getPort() const;

		static void select(std::vector<BaseSocket::SPtr> sockets, std::chrono::milliseconds timeout);

	protected:
		void BeginReceive();

		int _sockedFd;
		std::string _host;
		int _port;

		std::chrono::milliseconds _recvTimeout;
		BeginReceiveHandler _beginReceiveHandler;
	};
}
}


#endif  // EIPSCANNER_SOCKETS_BASESOCKET_H
