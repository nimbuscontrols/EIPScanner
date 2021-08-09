//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_EPATH_H
#define EIPSCANNER_CIP_EPATH_H

#include <cstdint>
#include <string>
#include <vector>

#include "Types.h"
#include "segments/ISegment.h"

namespace eipScanner {
namespace cip {
	class EPath {
	public:
		EPath();
		explicit EPath(CipUint classId);
		EPath(CipUint classId, CipUint objectId);
		EPath(CipUint classId, CipUint objectId, CipUint attributeId);
        EPath(const std::vector<segments::ISegment::SPtr> &segments);
		std::vector<uint8_t> packPaddedPath(bool use_8_bit_path_segments=false) const;
		void expandPaddedPath(const std::vector<uint8_t>& data);

		CipUint getClassId() const;
		CipUint getObjectId() const;
		CipUint getAttributeId() const;
		CipUsint getSizeInWords(bool use_8_bit_path_segments=false) const;

		std::string toString() const;
		bool operator==(const EPath& other) const;

	private:
		CipUint _classId;
		CipUint _objectId;
		CipUint _attributeId;
		CipUsint _size;
        std::vector<segments::ISegment::SPtr> _segments;
	};
}
}
#endif  // EIPSCANNER_CIP_EPATH_H
