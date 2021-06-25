//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_MESSAGEROUTERREQUEST_H
#define EIPSCANNER_CIP_MESSAGEROUTERREQUEST_H

#include <cstdint>
#include <vector>

#include "Services.h"
#include "EPath.h"

namespace eipScanner {
namespace cip {
	class MessageRouterRequest {
	public:
		MessageRouterRequest(CipUsint serviceCode, const EPath& ePath, const std::vector<uint8_t> data, bool use_8_bit_path_segments=false);
		~MessageRouterRequest();

		std::vector<uint8_t> pack() const;
	private:
		CipUsint _serviceCode;
		EPath _ePath;
		std::vector<uint8_t> _data;
        bool _use_8_bit_path_segments;
	};

}
}


#endif  // EIPSCANNER_CIP_MESSAGEROUTERREQUEST_H
