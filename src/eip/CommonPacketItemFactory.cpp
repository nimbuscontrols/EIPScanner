//
// Created by Aleksey Timin on 11/16/19.
//

#include "CommonPacketItemFactory.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace eip {
	using utils::Buffer;

	CommonPacketItem CommonPacketItemFactory::createNullAddressItem() const {
		return CommonPacketItem();
	}

	CommonPacketItem CommonPacketItemFactory::createUnconnectedDataItem(const std::vector<uint8_t> &data) const {
		return CommonPacketItem(CommonPacketItemIds::UNCONNECTED_MESSAGE, data);
	}

	CommonPacketItem CommonPacketItemFactory::createConnectedDataItem(const std::vector<uint8_t> &data) const {
		return CommonPacketItem(CommonPacketItemIds::CONNECTED_TRANSPORT_PACKET, data);
	}

	CommonPacketItem
	CommonPacketItemFactory::createSequenceAddressItem(cip::CipUdint connectionId, cip::CipUdint seqNumber) const{
		Buffer buffer;
		buffer << connectionId << seqNumber;
		return CommonPacketItem(CommonPacketItemIds::SEQUENCED_ADDRESS_ITEM, buffer.data());
	}
}
}