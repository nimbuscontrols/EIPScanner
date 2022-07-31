//
// Created by Aleksey Timin on 11/18/19.
//

#if defined(__unix__) || defined(__APPLE__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "utils/Logger.h"
#include "UDPSocket.h"
#include "Platform.h"

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
			throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
		}

		Logger(LogLevel::DEBUG) << "Opened UDP socket fd=" << _sockedFd;
	}

	UDPSocket::~UDPSocket() {
		Logger(LogLevel::DEBUG) << "Close UDP socket fd=" << _sockedFd;
		Shutdown();
		Close();
	}

	void UDPSocket::Send(const std::vector <uint8_t> &data) const {
		Logger(LogLevel::TRACE) << "Send " << data.size() << " bytes from UDP socket #" << _sockedFd << ".";

		const auto& addr = _remoteEndPoint.getAddr();
		int count = sendto(_sockedFd, (char*)data.data(), data.size(), 0,
				(struct sockaddr *)&addr, sizeof(addr));
		if (count < data.size()) {
			throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
		}
	}

	std::vector<uint8_t> UDPSocket::Receive(size_t size) const {
		std::vector<uint8_t> recvBuffer(size);

		auto len = recvfrom(_sockedFd, (char*)recvBuffer.data(), recvBuffer.size(), 0, NULL, NULL);
		Logger(LogLevel::TRACE) << "Received " << len << " bytes from UDP socket #" << _sockedFd << ".";
		if (len < 0) {
			throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
		}

		return recvBuffer;
	}

	std::vector<uint8_t> UDPSocket::ReceiveFrom(size_t size, EndPoint& endPoint) const {
		std::vector<uint8_t> recvBuffer(size);
		struct sockaddr_in addr;
		socklen_t addrFromLength = sizeof(addr);
		auto len = recvfrom(_sockedFd, (char*)recvBuffer.data(), recvBuffer.size(), 0, (struct sockaddr*)&addr, &addrFromLength);
		Logger(LogLevel::TRACE) << "Received " << len << " bytes from UDP socket #" << _sockedFd << ".";
		if (len < 0) {
			throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
		}

		endPoint = EndPoint(addr);
		return recvBuffer;
	}
}
}
