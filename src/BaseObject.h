//
// Created by Aleksey Timin on 12/9/19.
//

#ifndef EIPSCANNER_BASEOBJECT_H
#define EIPSCANNER_BASEOBJECT_H

#include "cip/Types.h"

namespace eipScanner {
	class BaseObject {
	public:
		BaseObject(cip::CipUint classId, cip::CipUint instanceId);

		cip::CipUint getClassId() const;
		cip::CipUint getInstanceId() const;

	private:
		cip::CipUint _classId;
		cip::CipUint _instanceId;
	};
}

#endif  // EIPSCANNER_BASEOBJECT_H
