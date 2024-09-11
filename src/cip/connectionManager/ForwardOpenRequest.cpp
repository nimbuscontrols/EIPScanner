//
// Created by Aleksey Timin on 11/18/19.
//

#include "src/ForwardOpenRequest.h"

#include <utility>
#include <assert.h>
#include "src/utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	using utils::Buffer;

	ForwardOpenRequest::ForwardOpenRequest(ConnectionParameters params)
		: _connectionParameters{std::move(params)} {
	}

	ForwardOpenRequest::~ForwardOpenRequest() = default;

	std::vector<uint8_t> ForwardOpenRequest::pack() const {
		const size_t size = 36;
		Buffer buffer(size);
		CipUsint reserved = 0;

		CipUint o2tNetworkConnectionParams = static_cast<CipUint>(_connectionParameters.o2tNetworkConnectionParams);
		CipUint t2oNetworkConnectionParams = static_cast<CipUint>(_connectionParameters.t2oNetworkConnectionParams);

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
			<< o2tNetworkConnectionParams
			<< _connectionParameters.t2oRPI
			<< t2oNetworkConnectionParams
			<< _connectionParameters.transportTypeTrigger
			<< _connectionParameters.connectionPathSize;

		assert(buffer.size() == size);

		buffer << _connectionParameters.connectionPath;
		return buffer.data();
	}
}
}
}
