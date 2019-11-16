//
// Created by Aleksey Timin on 11/16/19.
//

#include "utils/Buffer.h"
#include "EPath.h"

namespace eipScanner {
namespace cip {
	using utils::Buffer;

	EPath::EPath(CipUint classId)
		: _classId{classId}
		, _objectId{0}
		, _attributeId{0}
		, _size{1} {

	}

	EPath::EPath(CipUint classId, CipUint objectId)
			: _classId{classId}
			, _objectId{objectId}
			, _attributeId{0}
			, _size{2} {

	}

	EPath::EPath(CipUint classId, CipUint objectId, CipUint attributeId)
			: _classId{classId}
			, _objectId{objectId}
			, _attributeId{attributeId}
			, _size{3} {
	}

	std::vector<uint8_t> EPath::packPaddedPath() const {
		Buffer buffer(_size*4);

		CipUint classSegment = 0x21;
		buffer << classSegment << _classId;

		if (_size > 1) {
			CipUint objectSegment = 0x25;
			buffer << objectSegment << _objectId;

			if (_size > 2) {
				CipUint attributeSegment = 0x31;
				buffer << attributeSegment << _attributeId;
			}
		}

		return buffer.data();
	}

	CipUint EPath::getClassId() const {
		return _classId;
	}

	CipUint EPath::getObjectId() const {
		return _objectId;
	}

	CipUint EPath::getAttributeId() const {
		return _attributeId;
	}

	CipUsint EPath::getSizeInWords() const {
		return _size*2;
	}

	std::string EPath::toString() const {
		std::string msg = "[classId=" + std::to_string(_classId);
		if (_size > 1) {
			msg += " objectId=" + std::to_string(_objectId);
			if (_size > 2) {
				msg += " attributeId=" + std::to_string(_attributeId);
			}
		}

		msg += "]";
		return msg;
	}
}
}