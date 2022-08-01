//
// Created by Aleksey Timin on 12/10/19.
//

#ifndef EIPSCANNER_SOCKETS_ENDPOINT_H
#define EIPSCANNER_SOCKETS_ENDPOINT_H

#if defined(__unix__) || defined(__APPLE__)
#include <netinet/in.h>
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#endif

#include <string>

#define EIP_DEFAULT_EXPLICIT_PORT (uint16_t)(44818)
#define EIP_DEFAULT_IMPLICIT_PORT (uint16_t)(2222)

namespace eipScanner {
namespace sockets {

	class EndPoint {
	public:
		EndPoint(std::string  host, int port);
		EndPoint(struct sockaddr_in& addr);

		const std::string &getHost() const;
		int getPort() const;
		const sockaddr_in &getAddr() const;

		std::string toString() const;

		bool operator==(const EndPoint &rhs) const;
		bool operator!=(const EndPoint &rhs) const;
		bool operator< (const EndPoint& rhs) const;
	private:
		std::string _host;
		int _port;
		struct sockaddr_in _addr;
	};
}
}

#endif  // EIPSCANNER_ENDPOINT_H
