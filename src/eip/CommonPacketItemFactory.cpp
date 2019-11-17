//
// Created by Aleksey Timin on 11/16/19.
//

#include "CommonPacketItemFactory.h"

namespace eipScanner {
namespace eip {

	CommonPacketItem CommonPacketItemFactory::createNullAddressItem() const {
		return CommonPacketItem();
	}

	CommonPacketItem CommonPacketItemFactory::createUnconnectedDataItem(const std::vector<uint8_t> &data) const {
		return CommonPacketItem(CommonPacketItemIds::UNCONNECTED_MESSAGE, data);
	}
}
}