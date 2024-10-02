//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDOPENRESPONSE_H
#define EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDOPENRESPONSE_H

#include <vector>
#include "EIPScanner/cip/Types.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	class ForwardOpenResponse {
	public:
		ForwardOpenResponse();
		~ForwardOpenResponse();
		void expand(const std::vector<uint8_t>& data);

		CipUdint getO2TNetworkConnectionId() const;
		CipUdint getT2ONetworkConnectionId() const;
		CipUint getConnectionSerialNumber() const;
		CipUint getOriginatorVendorId() const;
		CipUdint getOriginatorSerialNumber() const;
		CipUdint getO2TApi() const;
		CipUdint getT2OApi() const;
		CipUsint getApplicationReplaySize() const;

		const std::vector<uint8_t> &getApplicationReplay() const;

	private:
		CipUdint _o2tNetworkConnectionId;
		CipUdint _t2oNetworkConnectionId;
		CipUint _connectionSerialNumber;
		CipUint _originatorVendorId;
		CipUdint _originatorSerialNumber;
		CipUdint _o2tAPI;
		CipUdint _t2oAPI;
		CipUsint _applicationReplaySize;
		std::vector<uint8_t> _applicationReplay;
	};

}
}
}

#endif  // EIPSCANNER_CIP_CONNECTIONMANAGER_FORWARDOPENRESPONSE_H
