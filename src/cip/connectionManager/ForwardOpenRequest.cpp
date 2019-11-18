//
// Created by Aleksey Timin on 11/18/19.
//

#include "ForwardOpenRequest.h"

#include <utility>
#include <assert.h>
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	using utils::Buffer;

	ForwardOpenRequest::ForwardOpenRequest(ConnectionParameters params)
		: _connectionParameters{std::move(params)} {
	}

	ForwardOpenRequest::~ForwardOpenRequest() = default;

	std::vector<uint8_t> ForwardOpenRequest::pack() const {
		Buffer buffer(36);
		CipUsint reserved = 0;

		buffer << _connectionParameters.priorityTimeTick
			<< _connectionParameters.timeoutTicks
			<< _connectionParameters.o2tNetworkConnectionId
			<< _connectionParameters.t2oNetworkConnectionId
			<< _connectionParameters.connectionSerialNumber
			<< _connectionParameters.originatorVendorId
			<< _connectionParameters.originatorSerialNumber
			<< _connectionParameters.connectionTimeoutMultiplier
			<< reserved << reserved << reserved
			<< _connectionParameters.o2tRPI
			<< _connectionParameters.o2tNetworkConnectionParams
			<< _connectionParameters.t2oRPI
			<< _connectionParameters.t2oNetworkConnectionParams
			<< _connectionParameters.transportTypeTrigger
			<< _connectionParameters.connectionPathSize;

		assert(buffer.size() == 36);

		buffer << _connectionParameters.connectionPath;
		return buffer.data();
	}
}
}
}