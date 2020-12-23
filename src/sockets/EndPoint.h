//
// Created by Aleksey Timin on 12/10/19.
//

#ifndef EIPSCANNER_SOCKETS_ENDPOINT_H
#define EIPSCANNER_SOCKETS_ENDPOINT_H

#ifdef __linux
#include <netinet/in.h>
#elif defined _WIN32
#include <winsock2.h>
#endif
#include <string>

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
