//
// Created by Aleksey Timin on 11/17/19.
//

#include <gtest/gtest.h>
#include "eip/CommonPacketItem.h"
#include "eip/CommonPacketItemFactory.h"
#include "eip/CommonPacket.h"

using eipScanner::eip::CommonPacket;
using eipScanner::eip::CommonPacketItem;
using eipScanner::eip::CommonPacketItemFactory;
using eipScanner::eip::CommonPacketItemIds;

TEST(CommonPacket, ShouldExpandFromData) {
	auto item1 = CommonPacketItemFactory().createNullAddressItem().pack();
	auto item2 = CommonPacketItemFactory().createUnconnectedDataItem({0x0, 0x2}).pack();

	std::vector<uint8_t> data = {0x2, 0x0};
	data.insert(data.end(), item1.begin(), item1.end());
	data.insert(data.end(), item2.begin(), item2.end());


	CommonPacket cp;
	cp.expand(data);

	EXPECT_EQ(cp[0].getTypeId(), CommonPacketItemIds::NULL_ADDR);
	EXPECT_EQ(cp[1].getTypeId(), CommonPacketItemIds::UNCONNECTED_MESSAGE);
}

TEST(CommonPacket, ShouldThrowErrorIfDataIsInvalid) {
	auto item1 = CommonPacketItemFactory().createUnconnectedDataItem({}).pack();
	auto item2 = CommonPacketItemFactory().createUnconnectedDataItem({}).pack();

	std::vector<uint8_t> invalidData = {0x2, 0x0};
	invalidData.insert(invalidData.end(), item1.begin(), item1.end());
	invalidData.insert(invalidData.end(), item2.begin(), item2.end());
	invalidData.pop_back();

	CommonPacket cp;
	EXPECT_THROW(cp.expand(invalidData), std::runtime_error);
}