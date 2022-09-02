//
// Created by Aleksey Timin on 11/16/19.
//

#include <gtest/gtest.h>

#include "eip/CommonPacketItem.h"
#include "eip/CommonPacketItemFactory.h"

using eipScanner::eip::CommonPacketItem;
using eipScanner::eip::CommonPacketItemFactory;

TEST(CommonPacketItem, UnconnectedDataItem) {
	std::vector<uint8_t> data = {1, 2, 3, 4};
	CommonPacketItem item = CommonPacketItemFactory().createUnconnectedDataItem(data);

	std::vector<uint8_t> expectedData = {0xB2, 0, 4, 0, 1, 2, 3, 4};

	EXPECT_EQ(item.pack(), expectedData);
}

TEST(CommonPacketItem, NullAddressItem) {
	CommonPacketItem item = CommonPacketItemFactory().createNullAddressItem();

	std::vector<uint8_t> expectedData = {0, 0, 0, 0};

	EXPECT_EQ(item.pack(), expectedData);
}
