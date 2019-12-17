//
// Created by Aleksey Timin on 12/17/19.
//

#ifndef EIPSCANNER_DISCOVERYMANAGER_H
#define EIPSCANNER_DISCOVERYMANAGER_H

#include "IdentityObject.h"

namespace eipScanner {

	struct IdentityItem {
		using Vec = std::vector<IdentityItem>;

		IdentityObject identityObject;
		sockets::EndPoint socketAddress;
	};

	class DiscoveryManager {
	public:
		explicit DiscoveryManager(const std::string& broadCastAddress, int port, std::chrono::milliseconds receiveTimout);
		~DiscoveryManager();

		IdentityItem::Vec discovery() const;
	protected:
		virtual sockets::BaseSocket::UPtr makeSocket() const;

	private:
		sockets::EndPoint _broadCastAddress;
		std::chrono::milliseconds _receiveTimout;
	};
}

#endif  //EIPSCANNER_DISCOVERYMANAGER_H
