//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_SERVICES_H
#define EIPSCANNER_CIP_SERVICES_H

#include "Types.h"

namespace eipScanner {
namespace cip {

	enum ServiceCodes : CipUsint {
		NONE = 0x00,
		/* Start CIP common services */
		GET_ATTRIBUTE_ALL = 0X01,
		SET_ATTRIBUTE_ALL = 0X02,
		GET_ATTRIBUTE_SINGLE = 0X0E,
		SET_ATTRIBUTE_SINGLE = 0X10,
		/* End CIP common services */
	};

}
}

#endif //EIPSCANNER_SERVICES_H
