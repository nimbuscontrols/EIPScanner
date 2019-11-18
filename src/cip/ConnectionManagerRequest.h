//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_CIP_CONNECTIONMANAGERREQUEST_H
#define EIPSCANNER_CIP_CONNECTIONMANAGERREQUEST_H

#include <vector>
#include "ConnectionParameters.h"

namespace eipScanner {
namespace cip {
	class ConnectionManagerRequest {
	public:
		ConnectionManagerRequest(ConnectionParameters  params);
		~ConnectionManagerRequest();
		std::vector<uint8_t> pack() const;

	private:
		ConnectionParameters _connectionParameters;
	};
}
}
#endif  // EIPSCANNER_CIP_CONNECTIONMANAGERREQUEST_H
