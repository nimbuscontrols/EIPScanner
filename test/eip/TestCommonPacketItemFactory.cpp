//
// Created by Aleksey Timin on 11/16/19.
//

#include <gtest/gtest.h>

#include "eip/CommonPacketItemFactory.h"

using eipScanner::eip::CommonPacketItemFactory;
using eipScanner::eip::CommonPacketItemIds;

TEST(CommonPacketItemFactory, ShouldCreateUnconnectedDataItem) {
	std::vector<uint8_t> data = {1,2,3,4};
	auto item = CommonPacketItemFactory().createUnconnectedDataItem(data);

	std::vector<uint8_t>  exp_data = { 0xB2, 0, 4, 0, 1, 2, 3, 4 };
	EXPECT_EQ(item.getTypeId(), CommonPacketItemIds::UNCONNECTED_MESSAGE);
	EXPECT_EQ(item.getLength(), data.size());
	EXPECT_EQ(item.getData(), data);
	EXPECT_EQ(item.pack(),  exp_data);
}

TEST(CommonPacketItemFactory, ShouldCreateNullAddressItem) {
	auto item = CommonPacketItemFactory().createNullAddressItem();

	EXPECT_EQ(item.getTypeId(), CommonPacketItemIds::NULL_ADDR);
	EXPECT_EQ(item.getLength(), 0);
}
