//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_CIP_CONNECTIONPARAMETERS_H
#define EIPSCANNER_CIP_CONNECTIONPARAMETERS_H

#include "cip/Types.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	struct ConnectionParameters {
		CipUsint priorityTimeTick = 0;
		CipUsint timeoutTicks = 0;
		CipUdint o2tNetworkConnectionId = 0;
		CipUdint t2oNetworkConnectionId = 0;
		CipUint connectionSerialNumber = 0;
		CipUint originatorVendorId = 0;
		CipUdint originatorSerialNumber = 0;
		CipUsint connectionTimeoutMultiplier = 0;
		CipUdint o2tRPI = 0;
		CipUdint o2tNetworkConnectionParams = 0;
		CipUdint t2oRPI = 0;
		CipUdint t2oNetworkConnectionParams = 0;
		CipUsint transportTypeTrigger = 0;
		CipUsint connectionPathSize = 0;
		CipBool o2tRealTimeFormat = 0;
		CipBool t2oRealTimeFormat = 0;
		std::vector<uint8_t> connectionPath = {};
	};
}
}
}

#endif //EIPSCANNER_CIP_CONNECTIONPARAMETERS_H
