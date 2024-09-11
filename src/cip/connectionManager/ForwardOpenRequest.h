//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDOPENREQUEST_H
#define EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDOPENREQUEST_H

#include <vector>
#include "src/ConnectionParameters.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {
	class ForwardOpenRequest {
	public:
		ForwardOpenRequest(ConnectionParameters  params);
		~ForwardOpenRequest();
		std::vector<uint8_t> pack() const;

	private:
		ConnectionParameters _connectionParameters;
	};
}
}
}
#endif  // EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDOPENREQUEST_H
