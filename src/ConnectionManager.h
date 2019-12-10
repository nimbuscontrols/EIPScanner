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
#include "sockets/UDPBoundSocket.h"

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
		FORWARD_OPEN = 0x54,
		FORWARD_CLOSE = 0x4E
	};

	class ConnectionManager {
	public:
		explicit ConnectionManager(MessageRouter::SPtr messageRouter);
		~ConnectionManager();

		IOConnection::WPtr forwardOpen(SessionInfoIf::SPtr si, cip::connectionManager::ConnectionParameters connectionParameters);
		void forwardClose(SessionInfoIf::SPtr si, const IOConnection::WPtr& ioConnection);

		/**
		 * Handles active connections
		 * @param timeout the timeout of receiving the data by select function
		 */
		void handleConnections(std::chrono::milliseconds timeout);
		bool hasOpenConnections() const;
	private:
		MessageRouter::SPtr _messageRouter;
		std::map<cip::CipUint, IOConnection::SPtr> _connectionMap;
		std::map<SocketKey, std::shared_ptr<sockets::UDPBoundSocket>> _socketMap;

		sockets::UDPBoundSocket::SPtr  findOrCreateSocket(const std::string& host, int port);
		std::chrono::steady_clock::time_point _lastHandleTime;
	};
}

#endif  // EIPSCANNER_CONNECTIONMANAGER_H
