//
// Created by Aleksey Timin on 11/18/19.
//

#include "ConnectionManagerResponse.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {
	using utils::Buffer;

	ConnectionManagerResponse::ConnectionManagerResponse()
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

	ConnectionManagerResponse::~ConnectionManagerResponse() = default;

	void ConnectionManagerResponse::expand(const std::vector<uint8_t> &data) {
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

	CipUdint ConnectionManagerResponse::getO2TNetworkConnectionId() const {
		return _o2tNetworkConnectionId;
	}

	CipUdint ConnectionManagerResponse::getT2ONetworkConnectionId() const {
		return _t2oNetworkConnectionId;
	}

	CipUint ConnectionManagerResponse::getConnectionSerialNumber() const {
		return _connectionSerialNumber;
	}

	CipUint ConnectionManagerResponse::getOriginatorVendorId() const {
		return _originatorVendorId;
	}

	CipUdint ConnectionManagerResponse::getOriginatorSerialNumber() const {
		return _originatorSerialNumber;
	}

	CipUdint ConnectionManagerResponse::getO2TApi() const {
		return _o2tAPI;
	}

	CipUdint ConnectionManagerResponse::getT2OApi() const {
		return _t2oAPI;
	}

	CipUsint ConnectionManagerResponse::getApplicationReplaySize() const {
		return _applicationReplaySize;
	}

	const std::vector<uint8_t> &ConnectionManagerResponse::getApplicationReplay() const {
		return _applicationReplay;
	}
}
}