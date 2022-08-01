//
// Created by Vincent Prince on 05/22/20.
//

#ifndef EIPSCANNER_CIP_CONNECTIONMANAGER_LARGEFORWARDOPENREQUEST_H
#define EIPSCANNER_CIP_CONNECTIONMANAGER_LARGEFORWARDOPENREQUEST_H

#include <vector>

#include "ConnectionParameters.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {
	class LargeForwardOpenRequest {
	public:
		LargeForwardOpenRequest(ConnectionParameters  params);
		~LargeForwardOpenRequest();
		std::vector<uint8_t> pack() const;

	private:
		ConnectionParameters _connectionParameters;
	};
}
}
}

#endif  // EIPSCANNER_CIP_CONNECTIONMANAGER_LARGEFORWARDOPENREQUEST_H
