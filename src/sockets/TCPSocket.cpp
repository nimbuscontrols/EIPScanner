//
// Created by Aleksey Timin on 11/16/19.
//

#include <system_error>

#if defined(__unix__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#endif

#include <unistd.h>
#include <fcntl.h>

#include "utils/Logger.h"
#include "TCPSocket.h"
#include "Platform.h"

namespace eipScanner {
	namespace sockets {
		using eipScanner::utils::Logger;
		using eipScanner::utils::LogLevel;

		TCPSocket::TCPSocket(std::string host, int port)
				: TCPSocket(EndPoint(host, port)) {
		}

		TCPSocket::TCPSocket(EndPoint endPoint, std::chrono::milliseconds connTimeout)
				: BaseSocket(std::move(endPoint)) {
			_sockedFd = socket(AF_INET, SOCK_STREAM, 0);
			if (_sockedFd < 0) {
				throw std::system_error(SOCKET_ERRNO(), std::generic_category());
			}

			// Set non-blocking
#if defined(__unix__)
			auto arg = fcntl(_sockedFd, F_GETFL, NULL);
			if (arg < 0) {
				throw std::system_error(SOCKET_ERRNO(), std::generic_category());
			}

			arg |= O_NONBLOCK;
			if (fcntl(_sockedFd, F_SETFL, arg) < 0) {
				throw std::system_error(SOCKET_ERRNO(), std::generic_category());
			}
#endif

			Logger(LogLevel::DEBUG) << "Opened TCP socket fd=" << _sockedFd;

			Logger(LogLevel::DEBUG) << "Connecting to " << _remoteEndPoint.toString();
			auto addr = _remoteEndPoint.getAddr();
			auto res = connect(_sockedFd, (struct sockaddr *) &addr, sizeof(addr));
			if (res < 0) {
				if (SOCKET_ERRNO() == EINPROGRESS) {
					do {
						fd_set myset;
						auto tv = makePortableInterval(connTimeout);

						FD_ZERO(&myset);
						FD_SET(_sockedFd, &myset);
						res = ::select(_sockedFd + 1, NULL, &myset, NULL, &tv);

						if (res < 0 && SOCKET_ERRNO() != EINTR) {
							throw std::system_error(SOCKET_ERRNO(), std::generic_category());
						} else if (res > 0) {
							// Socket selected for write
							int err;
							socklen_t lon = sizeof(int);
							if (getsockopt(_sockedFd, SOL_SOCKET, SO_ERROR, (char *) (&err), &lon) < 0) {
								throw std::system_error(SOCKET_ERRNO(), std::generic_category());
							}
							// Check the value returned...
							if (err) {
								throw std::system_error(err, std::generic_category());
							}
							break;
						} else {
							throw std::system_error(ETIMEDOUT, std::generic_category());
						}
					} while (1);
				} else {
					throw std::system_error(SOCKET_ERRNO(), std::generic_category());
				}
			}

#if defined(__unix__)
			// Set to blocking mode again...
			if ((arg = fcntl(_sockedFd, F_GETFL, NULL)) < 0) {
				throw std::system_error(SOCKET_ERRNO(), std::generic_category());
			}
			arg &= (~O_NONBLOCK);
			if (fcntl(_sockedFd, F_SETFL, arg) < 0) {
				throw std::system_error(SOCKET_ERRNO(), std::generic_category());
			}
#endif
		}


		TCPSocket::TCPSocket(EndPoint endPoint)
				: TCPSocket(std::move(endPoint), std::chrono::seconds(1)) {

		}

		TCPSocket::~TCPSocket() {
			Logger(LogLevel::DEBUG) << "Close TCP socket fd=" << _sockedFd;
			shutdown(_sockedFd, SOCKET_SHUTDOWN_OPERATION);
			close(_sockedFd);
		}

		void TCPSocket::Send(const std::vector<uint8_t> &data) const {
			Logger(LogLevel::TRACE) << "Send " << data.size() << " bytes from TCP socket #" << _sockedFd << ".";

			int count = send(_sockedFd, (char*)data.data(), data.size(), 0);
			if (count < data.size()) {
				throw std::system_error(SOCKET_ERRNO(), std::generic_category());
			}
		}

		std::vector<uint8_t> TCPSocket::Receive(size_t size) const {
			std::vector<uint8_t> recvBuffer(size);

			int count = 0;
			while (size > count) {
				auto len = recv(_sockedFd, (char*)(recvBuffer.data() + count), size - count, 0);
				Logger(LogLevel::TRACE) << "Received " << len << " bytes from TCP socket #" << _sockedFd << ".";
				count += len;
				if (len < 0) {
					throw std::system_error(SOCKET_ERRNO(), std::generic_category());
				}

				if (len == 0) {
					break;
				}
			}

			if (size != count) {
				Logger(LogLevel::WARNING) << "Received from " << _remoteEndPoint.toString()
										  << " " << count << " of " << size;
			}

			return recvBuffer;
		}
	}
}
