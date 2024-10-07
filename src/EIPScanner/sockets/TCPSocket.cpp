//
// Created by Aleksey Timin on 11/16/19.
//

#if defined(__unix__) || defined(__APPLE__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#endif

#include <fcntl.h>

#include "EIPScanner/utils/Logger.h"
#include "EIPScanner/sockets/TCPSocket.h"
#include "EIPScanner/sockets/Platform.h"

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
				throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
			}

#ifdef SO_NOSIGPIPE
			// Do not generate SIGPIPE for this socket
			if (setsockopt(_sockedFd, SOL_SOCKET, SO_NOSIGPIPE, &(int){ 1 }, sizeof(int)) < 0) {
				throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
			}
#endif

			// Set non-blocking
#if defined(__unix__) || defined(__APPLE__)
			auto arg = fcntl(_sockedFd, F_GETFL, NULL);
			if (arg < 0) {
				throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
			}

			arg |= O_NONBLOCK;
			if (fcntl(_sockedFd, F_SETFL, arg) < 0) {
				throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
			}
#endif

			Logger(LogLevel::DEBUG) << "Opened TCP socket fd=" << _sockedFd;

			Logger(LogLevel::DEBUG) << "Connecting to " << _remoteEndPoint.toString();
			try {
				auto addr = _remoteEndPoint.getAddr();
				auto res = connect(_sockedFd, (struct sockaddr *) &addr, sizeof(addr));
				if (res < 0) {
					if (BaseSocket::getLastError() == EIPSCANNER_SOCKET_ERROR(EINPROGRESS)) {
						do {
							fd_set myset;
							auto tv = makePortableInterval(connTimeout);

							FD_ZERO(&myset);
							FD_SET(_sockedFd, &myset);
							res = ::select(_sockedFd + 1, NULL, &myset, NULL, &tv);

							if (res < 0 && BaseSocket::getLastError() != EIPSCANNER_SOCKET_ERROR(EINTR)) {
								throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
							} else if (res > 0) {
								// Socket selected for write
								int err;
								socklen_t lon = sizeof(int);
								if (getsockopt(_sockedFd, SOL_SOCKET, SO_ERROR, (char *) (&err), &lon) < 0) {
									throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
								}
								// Check the value returned...
								if (err) {
									throw std::system_error(err, BaseSocket::getErrorCategory());
								}
								break;
							} else {
								throw std::system_error(EIPSCANNER_SOCKET_ERROR(ETIMEDOUT), BaseSocket::getErrorCategory());
							}
						} while (1);
					} else {
						throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
					}
				}

#if defined(__unix__) || defined(__APPLE__)
				// Set to blocking mode again...
				if ((arg = fcntl(_sockedFd, F_GETFL, NULL)) < 0) {
					throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
				}
				arg &= (~O_NONBLOCK);
				if (fcntl(_sockedFd, F_SETFL, arg) < 0) {
					throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
				}
#endif
			} catch (...) {
				Close();
				throw;
			}

		}


		TCPSocket::TCPSocket(EndPoint endPoint)
				: TCPSocket(std::move(endPoint), std::chrono::seconds(1)) {

		}

		TCPSocket::~TCPSocket() {
			Logger(LogLevel::DEBUG) << "Close TCP socket fd=" << _sockedFd;
			Shutdown();
			Close();
		}

		void TCPSocket::Send(const std::vector<uint8_t> &data) const {
			Logger(LogLevel::TRACE) << "Send " << data.size() << " bytes from TCP socket #" << _sockedFd << ".";

			int flags = 0;
#ifdef MSG_NOSIGNAL
			// Do not generate SIGPIPE when calling send() on closed socket
			flags |= MSG_NOSIGNAL;
#endif
			int count = send(_sockedFd, (char*)data.data(), data.size(), flags);
			if (count < data.size()) {
				throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
			}
		}

		std::vector<uint8_t> TCPSocket::Receive(size_t size) const {
			std::vector<uint8_t> recvBuffer(size);

			int count = 0;
			while (size > count) {
				auto len = recv(_sockedFd, (char*)(recvBuffer.data() + count), size - count, 0);
				if (len < 0) {
					// Check for EINTR, in which case, better luck next time
					if (BaseSocket::getLastError() == EIPSCANNER_SOCKET_ERROR(EINTR)) {
						continue;
					}
					throw std::system_error(BaseSocket::getLastError(), BaseSocket::getErrorCategory());
				}

				Logger(LogLevel::TRACE) << "Received " << len << " bytes from TCP socket #" << _sockedFd << ".";
				count += len;

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
