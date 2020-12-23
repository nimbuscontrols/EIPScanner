//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_SOCKETS_UDPSOCKET_H
#define EIPSCANNER_SOCKETS_UDPSOCKET_H

#include <vector>
#include <chrono>
#include <memory>
#include "BaseSocket.h"

namespace eipScanner {
namespace sockets {

	class UDPSocket : public BaseSocket{
	public:
		using WPtr = std::weak_ptr<UDPSocket>;
		using SPtr = std::shared_ptr<UDPSocket>;
		using UPtr = std::unique_ptr<UDPSocket>;

		explicit UDPSocket(EndPoint endPoint);
		UDPSocket(std::string host, int port);
		virtual ~UDPSocket();

		void Send(const std::vector<uint8_t>& data) const override;
		std::vector<uint8_t> Receive(size_t size) const override ;
		std::vector<uint8_t> ReceiveFrom(size_t size, EndPoint& endPoint) const ;
	};
}
}

#endif  // EIPSCANNER_SOCKETS_UDPSOCKET_H

