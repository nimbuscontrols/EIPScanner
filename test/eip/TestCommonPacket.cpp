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

TEST(CommonPacket, ShouldThrowErrorIfDataIsInvalid) {
	CommonPacketItem item1 = CommonPacketItemFactory().createUnconnectedDataItem({});
	CommonPacketItem item2 = CommonPacketItemFactory().createUnconnectedDataItem({});

	auto invalidData = item1.pack();
	invalidData.insert(invalidData.end(), item2.pack().begin(), item2.pack().end());
	invalidData.pop_back();

	CommonPacket cp;
	EXPECT_THROW(cp.expand(invalidData), std::runtime_error);
}