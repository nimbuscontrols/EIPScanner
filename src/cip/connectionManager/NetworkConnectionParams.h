//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_CIP_CONNECTIONMANAGER_NETWORKCONNECTIONPARAMS_H
#define EIPSCANNER_CIP_CONNECTIONMANAGER_NETWORKCONNECTIONPARAMS_H

#include "cip/Types.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	enum NetworkConnectionParams : CipUint {
		// Redundant Owner
		REDUNDANT = (1 << 15),
		OWNED = 0,
		TYPE0 = 0,

		// Connection type.
		MULTICAST = (1 << 13),
		P2P = (2 << 13),

		// Priorities
		LOW_PRIORITY = 0,
		HIGH_PRIORITY = (1 << 10),
		SCHEDULED_PRIORITY = (2 << 10),
		URGENT = (3 << 10),

		// Type of size.
		FIXED = 0,
		VARIABLE = (1 << 9),

		// Type of trigger.
		TRIG_CYCLIC = 0,
		TRIG_CHANGE = (1 << 4),
		TRIG_APP = (2 << 4),
		TRANSP_SERVER = 0x80
	};

}
}
}
#endif //EIPSCANNER_CIP_CONNECTIONMANAGER_NETWORKCONNECTIONPARAMS_H
