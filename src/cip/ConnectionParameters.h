//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_CIP_CONNECTIONPARAMETERS_H
#define EIPSCANNER_CIP_CONNECTIONPARAMETERS_H

#include "Types.h"

namespace eipScanner {
namespace cip {

	struct ConnectionParameters {
		CipUsint priority_time_tick = 0;
		CipUsint timeout_ticks = 0;
		CipUdint o2t_network_connection_id = 0;
		CipUdint t2o_network_connection_id = 0;
		CipUint connection_serial_number = 0;
		CipUint originator_vendor_id = 0;
		CipUdint originator_serial_number = 0;
		CipUsint connection_timeout_multiplier = 0;
		CipUdint o2t_rpi = 0;
		CipUint o2t_network_connection_params = 0;
		CipUdint t2o_rpi = 0;
		CipUint t2o_network_connection_params = 0;
		CipUsint transport_type_trigger = 0;
		CipUsint connection_path_size = 0;
		std::vector<uint8_t> connection_path = {};
	};
}
}
#endif //EIPSCANNER_CIP_CONNECTIONPARAMETERS_H
