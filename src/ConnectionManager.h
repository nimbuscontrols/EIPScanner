//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_CONNECTIONMANAGER_H
#define EIPSCANNER_CONNECTIONMANAGER_H

#include <map>
#include "MessageRouter.h"
#include "IOConnection.h"
#include "cip/connectionManager/ConnectionParameters.h"
#include "cip/Services.h"
#include "cip/Types.h"
#include "sockets/UDPSocket.h"

namespace eipScanner {

	struct SocketKey {
		std::string host;
		int port;

		inline bool operator< (const SocketKey& rhs) const {
			return host < rhs.host
				&& port < rhs.port;
		}
	};

	enum class ConnectionManagerServiceCodes : cip::CipUsint {
		FORWARD_OPEN = 0x54
	};

	class ConnectionManager {
	public:
		explicit ConnectionManager(MessageRouter::SPtr messageRouter);
		~ConnectionManager();

		IOConnection::WPtr forwardOpen(cip::connectionManager::ConnectionParameters connectionParameters);

		void handleConnections(std::chrono::milliseconds timeout);
		bool hasOpenConnections() const;

	private:
		MessageRouter::SPtr _messageRouter;
		std::map<cip::CipUint, IOConnection::SPtr> _connectionMap;
		std::map<SocketKey, std::shared_ptr<sockets::UDPSocket>> _socketMap;

		sockets::UDPSocket::SPtr  findOrCreateSocket(const std::string& host, int port);
	};
}

#endif  // EIPSCANNER_CONNECTIONMANAGER_H
