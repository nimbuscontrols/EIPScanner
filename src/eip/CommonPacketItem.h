//
// Created by flipback on 11/16/19.
//

#ifndef EIPSCANNER_EIP_COMMONPACKETITEM_H
#define EIPSCANNER_EIP_COMMONPACKETITEM_H

#include <vector>
#include <cstdint>

#include "cip/Types.h"

namespace eipScanner {
namespace eip {
	enum class CommonPacketItemIds : cip::CipUint {
		NULL_ADDR = 0x0000,
		LIST_IDENTITY = 0x000C,
		CONNECTION_ADDRESS_ITEM= 0x00A1,
		CONNECTED_TRANSPORT_PACKET = 0x00B1,
		UNCONNECTED_MESSAGE = 0x00B2,
		O2T_SOCKADDR_INFO = 0x8000,
		T2O_SOCKADDR_INFO = 0x8001,
		SEQUENCED_ADDRESS_ITEM= 0x8002,
	};

	class CommonPacketItem {
	public:
		using Vec = std::vector<CommonPacketItem>;

		CommonPacketItem();
		CommonPacketItem(CommonPacketItemIds typeId, const std::vector<uint8_t>& data);
		~CommonPacketItem();

		std::vector<uint8_t> pack() const;

		CommonPacketItemIds getTypeId() const;
		cip::CipUint getLength() const;
		const std::vector<uint8_t> &getData() const;

		bool operator==(const CommonPacketItem &rhs) const;

		bool operator!=(const CommonPacketItem &rhs) const;

	private:
		CommonPacketItemIds _typeId;
		cip::CipUint _length;
		std::vector<uint8_t> _data;
	};

}
}


#endif  // EIPSCANNER_EIP_COMMONPACKETITEM_H
