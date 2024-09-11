//
// Created by Aleksey Timin on 11/18/19.
//

#include "src/ForwardOpenResponse.h"
#include "src/utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	using utils::Buffer;

	ForwardOpenResponse::ForwardOpenResponse()
		: _o2tNetworkConnectionId{0}
		, _t2oNetworkConnectionId{0}
		, _connectionSerialNumber{0}
		, _originatorVendorId{0}
		, _originatorSerialNumber{0}
		, _o2tAPI{0}
		, _t2oAPI{0}
		, _applicationReplaySize{0}
		, _applicationReplay{0} {
	}

	ForwardOpenResponse::~ForwardOpenResponse() = default;

	void ForwardOpenResponse::expand(const std::vector<uint8_t> &data) {
		Buffer buffer(data);
		CipUsint reserved = 0;

		buffer >> _o2tNetworkConnectionId
			>> _t2oNetworkConnectionId
			>> _connectionSerialNumber
			>> _originatorVendorId
			>> _originatorSerialNumber
			>> _o2tAPI
			>> _t2oAPI
			>> _applicationReplaySize
			>> reserved;

		_applicationReplay.resize(_applicationReplaySize*2);
		buffer >> _applicationReplay;
	}

	CipUdint ForwardOpenResponse::getO2TNetworkConnectionId() const {
		return _o2tNetworkConnectionId;
	}

	CipUdint ForwardOpenResponse::getT2ONetworkConnectionId() const {
		return _t2oNetworkConnectionId;
	}

	CipUint ForwardOpenResponse::getConnectionSerialNumber() const {
		return _connectionSerialNumber;
	}

	CipUint ForwardOpenResponse::getOriginatorVendorId() const {
		return _originatorVendorId;
	}

	CipUdint ForwardOpenResponse::getOriginatorSerialNumber() const {
		return _originatorSerialNumber;
	}

	CipUdint ForwardOpenResponse::getO2TApi() const {
		return _o2tAPI;
	}

	CipUdint ForwardOpenResponse::getT2OApi() const {
		return _t2oAPI;
	}

	CipUsint ForwardOpenResponse::getApplicationReplaySize() const {
		return _applicationReplaySize;
	}

	const std::vector<uint8_t> &ForwardOpenResponse::getApplicationReplay() const {
		return _applicationReplay;
	}
}
}
}