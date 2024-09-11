//
// Created by Aleksey Timin on 11/16/19.
//

#include "src/utils/Buffer.h"
#include "src/eip/CommonPacketItem.h"

namespace eipScanner {
namespace eip {
	using utils::Buffer;

	CommonPacketItem::CommonPacketItem()
		: _typeId(CommonPacketItemIds::NULL_ADDR)
		, _length(0)
		, _data(0) {

	}

	CommonPacketItem::CommonPacketItem(CommonPacketItemIds typeId, const std::vector<uint8_t> &data)
		: _typeId{typeId}
		, _length(data.size())
		, _data(data) {

	}

	CommonPacketItem::~CommonPacketItem() = default;

	std::vector<uint8_t> CommonPacketItem::pack() const {
		Buffer buffer;

		buffer << static_cast<cip::CipUint>(_typeId) << _length;
		if (_length > 0) {
			buffer << _data;
		}
		return buffer.data();
	}

	CommonPacketItemIds CommonPacketItem::getTypeId() const {
		return _typeId;
	}

	cip::CipUint CommonPacketItem::getLength() const {
		return _length;
	}

	const std::vector<uint8_t> &CommonPacketItem::getData() const {
		return _data;
	}

	bool CommonPacketItem::operator==(const CommonPacketItem &rhs) const {
		return _typeId == rhs._typeId &&
			   _length == rhs._length &&
			   _data == rhs._data;
	}

	bool CommonPacketItem::operator!=(const CommonPacketItem &rhs) const {
		return !(rhs == *this);
	}
}
}
