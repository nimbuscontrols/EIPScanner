//
// Created by Aleksey Timin on 11/19/19.
//
#include "ForwardCloseRequest.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {
	using utils::Buffer;

	connectionManager::ForwardCloseRequest::ForwardCloseRequest()
		: _connectionSerialNumber{0}
		, _originatorSerialNumber{0}
		, _originatorVendorID{0}
		, _connectionPath(0) {

	}

	connectionManager::ForwardCloseRequest::~ForwardCloseRequest() = default;

	std::vector <uint8_t> connectionManager::ForwardCloseRequest::pack() const {
		Buffer buffer;
		CipUsint timeTick = 0;
		CipUsint timeOutTicks = 0;

		buffer << timeTick
			<< timeOutTicks
			<< _connectionSerialNumber
			<< _originatorVendorID
			<< _originatorSerialNumber
			<< static_cast<CipUsint>(_connectionPath.size()/2)
			<< _connectionPath;

		return buffer.data();
	}

	void ForwardCloseRequest::setConnectionSerialNumber(CipUint connectionSerialNumber) {
		_connectionSerialNumber = connectionSerialNumber;
	}

	void ForwardCloseRequest::setOriginatorVendorId(CipUint originatorVendorId) {
		_originatorVendorID = originatorVendorId;
	}

	void ForwardCloseRequest::setOriginatorSerialNumber(CipUdint originatorSerialNumber) {
		_originatorSerialNumber = originatorSerialNumber;
	}


	void ForwardCloseRequest::setConnectionPath(const std::vector<uint8_t> &connectionPath) {
		_connectionPath = connectionPath;
	}
}
}
}
