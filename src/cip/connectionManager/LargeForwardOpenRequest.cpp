//
// Created by Vincent Prince on 05/22/20.
//

#include "src/cip/connectionManager/LargeForwardOpenRequest.h"

#include <utility>
#include <assert.h>
#include "src/utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	using utils::Buffer;

	LargeForwardOpenRequest::LargeForwardOpenRequest(ConnectionParameters params)
		: _connectionParameters{std::move(params)} {
	}

	LargeForwardOpenRequest::~LargeForwardOpenRequest() = default;

	std::vector<uint8_t> LargeForwardOpenRequest::pack() const {
		const size_t size = 40;
		Buffer buffer(size);
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

		assert(buffer.size() == size);

		buffer << _connectionParameters.connectionPath;
		return buffer.data();
	}
}
}
}
