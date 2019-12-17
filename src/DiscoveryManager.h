//
// Created by Aleksey Timin on 12/17/19.
//

#ifndef EIPSCANNER_DISCOVERYMANAGER_H
#define EIPSCANNER_DISCOVERYMANAGER_H

#include "IdentityObject.h"

namespace eipScanner {

	/**
	 * @brief Contains information about EIP device and its address in the network
	 */
	struct IdentityItem {
		using Vec = std::vector<IdentityItem>;

		IdentityObject identityObject;
		sockets::EndPoint socketAddress;
	};

	/**
	 * @class DiscoveryManager
	 * @brief Implements the discovery of EIP devices in the network
	 */
	class DiscoveryManager {
	public:

		/**
		 * @brief Constructor
		 * @param broadCastAddress the broadcast address to send a request (e.g. 172.28.255.255 or 192.168.1.255)
		 * @param port the port of the discovery (default 0xAF12)
		 * @param receiveTimout the timeout to wait for responses from the EIP devices
		 */
		explicit DiscoveryManager(const std::string& broadCastAddress, int port, std::chrono::milliseconds receiveTimout);

		/**
		 * @brief default destructor
		 */
		~DiscoveryManager();

		/**
		 * @brief Discovers the EIP network
		 * Sends the broadcast request through UDP and collects all the response.
		 * @return the vector of IdentityItem for each discovered device.
		 */
		IdentityItem::Vec discover() const;
	protected:
		virtual sockets::BaseSocket::UPtr makeSocket() const;

	private:
		sockets::EndPoint _broadCastAddress;
		std::chrono::milliseconds _receiveTimout;
	};
}

#endif  //EIPSCANNER_DISCOVERYMANAGER_H
