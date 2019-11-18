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

namespace eipScanner {
	struct IOConnectionKey {
		cip::CipUdint	o2tNetworkId;
		cip::CipUdint	t2oNetworkId;
	};

	enum class ConnectionManagerServiceCodes : cip::CipUsint {
		FORWARD_OPEN = 0x54
	};

	class ConnectionManager {
	public:
		explicit ConnectionManager(MessageRouter::SPtr messageRouter);
		~ConnectionManager();

		IOConnection::WPtr forwardOpen(cip::connectionManager::ConnectionParameters connectionParameters);

	private:
		MessageRouter::SPtr _messageRouter;
		std::map<IOConnectionKey, IOConnection::SPtr> _connectionMap;
	};
}

#endif  // EIPSCANNER_CONNECTIONMANAGER_H
