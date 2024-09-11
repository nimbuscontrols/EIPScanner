//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_EIP_COMMONPACKET_H
#define EIPSCANNER_EIP_COMMONPACKET_H

#include "src/CommonPacketItem.h"

namespace eipScanner {
namespace eip {

	class CommonPacket {
	public:
		CommonPacket();
		~CommonPacket();

		CommonPacket& operator << (const CommonPacketItem& item);

		std::vector<uint8_t> pack() const;
		void expand(const std::vector<uint8_t>& data);
		const CommonPacketItem::Vec& getItems();
	private:
		CommonPacketItem::Vec _items;
	};

}
}

#endif  // EIPSCANNER_EIP_COMMONPACKET_H
