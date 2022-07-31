//
// Created by Aleksey Timin on 11/16/19.
//
#include <stdexcept>
#include "utils/Buffer.h"
#include "EPath.h"

namespace eipScanner {
namespace cip {
	using utils::Buffer;

	enum class EPathSegmentTypes : CipUsint  {
		CLASS_8_BITS = 0x20,
		CLASS_16_BITS = 0x21,
		INSTANCE_8_BITS = 0x24,
		INSTANCE_16_BITS = 0x25,
		ATTRIBUTE_8_BITS = 0x30,
		ATTRIBUTE_16_BITS = 0x31,
	};

	EPath::EPath()
			: _classId{0}
			, _objectId{0}
			, _attributeId{0}
			, _size{0}{

	}

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

	std::vector<uint8_t> EPath::packPaddedPath(bool use_8_bit_path_segments) const {
        if (use_8_bit_path_segments)
        {
            Buffer buffer(_size*2);

            auto classSegment = static_cast<CipUsint>(EPathSegmentTypes::CLASS_8_BITS);
            buffer << classSegment << static_cast<CipUsint>(_classId);

            if (_size > 1) {
                auto instanceSegment = static_cast<CipUsint>(EPathSegmentTypes::INSTANCE_8_BITS);
                buffer << instanceSegment << static_cast<CipUsint>(_objectId);

                if (_size > 2) {
                    auto attributeSegment = static_cast<CipUsint>(EPathSegmentTypes::ATTRIBUTE_8_BITS);
                    buffer << attributeSegment << static_cast<CipUsint>(_attributeId);
                }
            }

            return buffer.data();            
        }
        else
        {
            Buffer buffer(_size*4);

            auto classSegment = static_cast<CipUint>(EPathSegmentTypes::CLASS_16_BITS);
            buffer << classSegment << _classId;

            if (_size > 1) {
                auto instanceSegment = static_cast<CipUint>(EPathSegmentTypes::INSTANCE_16_BITS);
                buffer << instanceSegment << _objectId;

                if (_size > 2) {
                    auto attributeSegment = static_cast<CipUint>(EPathSegmentTypes::ATTRIBUTE_16_BITS);
                    buffer << attributeSegment << _attributeId;
                }
            }

            return buffer.data();
        }
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

	CipUsint EPath::getSizeInWords(bool use_8_bit_path_segments) const {
        if (use_8_bit_path_segments) {
            return _size;
        }
        else {
    		return _size*2;
		}
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

	void EPath::expandPaddedPath(const std::vector<uint8_t> &data) {
		Buffer buffer(data);

		_classId = 0;
		_objectId = 0;
		_attributeId = 0;
		_size = 0;

		for (size_t i = 0; i < data.size() && !buffer.empty(); ++i) {
			EPathSegmentTypes segmentType;
			CipUsint ignore = 0;
			CipUsint byte;
			CipUint word;
			buffer >> reinterpret_cast<CipUsint&>(segmentType);
			switch (segmentType) {
				case EPathSegmentTypes::CLASS_8_BITS:
					buffer >> byte;
					_classId = byte;
					break;
				case EPathSegmentTypes::CLASS_16_BITS:
					buffer >> ignore >> word;
					_classId = word;
					break;
				case EPathSegmentTypes::INSTANCE_8_BITS:
					buffer >> byte;
					_objectId = byte;
					break;
				case EPathSegmentTypes::INSTANCE_16_BITS:
					buffer >> ignore >> word;
					_objectId = word;
					break;
				case EPathSegmentTypes::ATTRIBUTE_8_BITS:
					buffer >> byte;
					_attributeId = byte;
					break;
				case EPathSegmentTypes::ATTRIBUTE_16_BITS:
					buffer >> ignore >> word;
					_attributeId = word;
					break;
				default:
					throw std::runtime_error("Unknown EPATH segment =" + std::to_string(static_cast<int>(segmentType)));
			}
		}

		if (!buffer.isValid()) {
			throw std::runtime_error("Wrong EPATH format");
		}

		if (_classId > 0) {
			_size++;

			if (_objectId > 0) {
				_size++;

				if (_attributeId > 0) {
					_size++;
				}
			}
		}
	}

	bool EPath::operator==(const EPath &other) const {
		return _size == other._size
			&&_classId == other._classId
			&& _objectId == other._objectId
			&& _attributeId == other._attributeId;
	}
}
}
