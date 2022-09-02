//
// Created by Aleksey Timin on 11/19/19.
//

#ifndef EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDCLOSEREQUEST_H
#define EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDCLOSEREQUEST_H

#include <vector>

#include "ConnectionParameters.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {
	class ForwardCloseRequest {
	public:
		ForwardCloseRequest();
		~ForwardCloseRequest();
		std::vector<uint8_t> pack() const;

		void setConnectionSerialNumber(CipUint connectionSerialNumber);
		void setOriginatorVendorId(CipUint originatorVendorId);
		void setOriginatorSerialNumber(CipUdint originatorSerialNumber);
		void setConnectionPath(const std::vector<uint8_t> &connectionPath);

	private:
		CipUint _connectionSerialNumber;
		CipUint _originatorVendorID;
		CipUdint _originatorSerialNumber;
		std::vector<uint8_t>_connectionPath;
	};
}
}
}

#endif  // EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDOPENREQUEST_H
