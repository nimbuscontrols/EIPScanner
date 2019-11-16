//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_EPATH_H
#define EIPSCANNER_CIP_EPATH_H

#include <vector>
#include <string>
#include <cstdint>

#include "Types.h"

namespace eipScanner {
namespace cip {
	class EPath {
	public:
		EPath(CipUint classId);
		EPath(CipUint classId, CipUint objectId);
		EPath(CipUint classId, CipUint objectId, CipUint attributeId);
		std::vector<uint8_t> packPaddedPath() const;

		CipUint getClassId() const;
		CipUint getObjectId() const;
		CipUint getAttributeId() const;
		CipUint getSize() const;

		std::string toString() const;

	private:
		CipUint _classId;
		CipUint _objectId;
		CipUint _attributeId;
		CipUint _size;
	};
}
}
#endif  // EIPSCANNER_CIP_EPATH_H
