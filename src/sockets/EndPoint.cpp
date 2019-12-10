//
// Created by Aleksey Timin on 12/10/19.
//

#include "EndPoint.h"
#include <arpa/inet.h>
#include <system_error>
#include <utility>

namespace eipScanner {
namespace sockets {

	EndPoint::EndPoint(struct sockaddr_in &addr)
		: _host(inet_ntoa(addr.sin_addr))
		, _port(htons(addr.sin_port))
		, _addr(addr) {
	}

	EndPoint::EndPoint(std::string host, int port)
		: _host(std::move(host))
		, _port(port)
		, _addr{0} {

		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(_port);
		if (inet_aton(_host.c_str(), &_addr.sin_addr) < 0) {
			throw std::system_error(errno, std::generic_category());
		}
	}

	const std::string &EndPoint::getHost() const {
		return _host;
	}

	int EndPoint::getPort() const {
		return _port;
	}

	const sockaddr_in &EndPoint::getAddr() const {
		return _addr;
	}

	bool EndPoint::operator==(const EndPoint &rhs) const {
		return _host == rhs._host &&
			   _port == rhs._port &&
			   _addr.sin_addr.s_addr == rhs._addr.sin_addr.s_addr &&
			   _addr.sin_port == rhs._addr.sin_port &&
			   _addr.sin_family == rhs._addr.sin_family;
	}

	bool EndPoint::operator!=(const EndPoint &rhs) const {
		return !(rhs == *this);
	}

	bool EndPoint::operator< (const EndPoint& rhs) const {
		return _host < rhs._host
			   && _port < rhs._port;
	}

	std::string EndPoint::toString() const {
		return _host + ":" + std::to_string(_port);
	}
}
}