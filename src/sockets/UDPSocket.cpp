//
// Created by Aleksey Timin on 11/18/19.
//

#include <system_error>

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#if !(defined __linux__) && !(defined SHUT_RDWR)
#define SHUT_RDWR SD_BOTH
#endif


#include <unistd.h>

#include "utils/Logger.h"
#include "UDPSocket.h"

namespace eipScanner {
namespace sockets {
	using eipScanner::utils::Logger;
	using eipScanner::utils::LogLevel;




	UDPSocket::UDPSocket(std::string host, int port)
		: UDPSocket(EndPoint(host, port)){

	}

	UDPSocket::UDPSocket(EndPoint endPoint)
			: BaseSocket(EndPoint(std::move(endPoint))) {

		_sockedFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (_sockedFd < 0) {
			throw std::system_error(errno, std::generic_category());
		}

		Logger(LogLevel::DEBUG) << "Opened socket fd=" << _sockedFd;
	}

	UDPSocket::~UDPSocket() {
		Logger(LogLevel::DEBUG) << "Close socket fd=" << _sockedFd;
		shutdown(_sockedFd, SHUT_RDWR);
		close(_sockedFd);
	}

	void UDPSocket::Send(const std::vector <uint8_t> &data) const {
		Logger(LogLevel::TRACE) << "Send " << data.size() << " bytes from UDP socket #" << _sockedFd << ".";

		auto addr = _remoteEndPoint.getAddr();
		int count = sendto(_sockedFd, (char*)data.data(), data.size(), 0,
				(struct sockaddr *)&addr, sizeof(addr));
		if (count < data.size()) {
			throw std::system_error(errno, std::generic_category());
		}
	}

	std::vector<uint8_t> UDPSocket::Receive(size_t size) const {
		std::vector<uint8_t> recvBuffer(size);

		auto len = recvfrom(_sockedFd, (char*)recvBuffer.data(), recvBuffer.size(), 0, NULL, NULL);
		if (len < 0) {
			throw std::system_error(errno, std::generic_category());
		}

		return recvBuffer;
	}

	std::vector<uint8_t> UDPSocket::ReceiveFrom(size_t size, EndPoint& endPoint) const {
		std::vector<uint8_t> recvBuffer(size);
		struct sockaddr_in addr;
		socklen_t addrFromLength = sizeof(addr);
		auto len = recvfrom(_sockedFd, (char*)recvBuffer.data(), recvBuffer.size(), 0, (struct sockaddr*)&addr, &addrFromLength);
		if (len < 0) {
			throw std::system_error(errno, std::generic_category());
		}

		endPoint = EndPoint(addr);
		return recvBuffer;
	}
}
}
