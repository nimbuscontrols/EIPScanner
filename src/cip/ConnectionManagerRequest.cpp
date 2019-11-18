//
// Created by Aleksey Timin on 11/18/19.
//

#include "ConnectionManagerRequest.h"

#include <utility>
#include <assert.h>
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {
	using utils::Buffer;

	ConnectionManagerRequest::ConnectionManagerRequest(ConnectionParameters params)
		: _connectionParameters{std::move(params)} {
	}

	ConnectionManagerRequest::~ConnectionManagerRequest() = default;

	std::vector<uint8_t> ConnectionManagerRequest::pack() const {
		Buffer buffer(36);
		CipUsint reserved = 0;

		buffer << _connectionParameters.priority_time_tick
			<< _connectionParameters.timeout_ticks
			<< _connectionParameters.o2t_network_connection_id
			<< _connectionParameters.t2o_network_connection_id
			<< _connectionParameters.connection_serial_number
			<< _connectionParameters.originator_vendor_id
			<< _connectionParameters.originator_serial_number
			<< _connectionParameters.connection_timeout_multiplier
			<< reserved << reserved << reserved
			<< _connectionParameters.o2t_rpi
			<< _connectionParameters.o2t_network_connection_params
			<< _connectionParameters.t2o_rpi
			<< _connectionParameters.t2o_network_connection_params
			<< _connectionParameters.connection_path_size;

		assert(buffer.size() == 36);

		buffer << _connectionParameters.connection_path;
		return buffer.data();
	}
}
}