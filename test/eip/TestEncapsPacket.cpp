//
// Created by Aleksey Timin on 11/16/19.
//

#include <gtest/gtest.h>
#include "eip/EncapsPacket.h"

using eipScanner::eip::EncapsPacket;
using eipScanner::eip::EncapsCommands;
using eipScanner::eip::EncapsStatusCodes;

TEST(TestEncapsPacket, ShouldExpandData) {
	std::vector<uint8_t> data = {0x6F, 0, 0xB, 0, 0xDD, 0xCC, 0xBB, 0xAA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										   0, 0, 0, 0, 0, 0, 0, 0,
										   0x64, 0, 1, 2, 3, 4, 5};

	EncapsPacket packet;
	packet.expand(data);

	EXPECT_EQ(EncapsCommands::SEND_RR_DATA, packet.getCommand());
	EXPECT_EQ(11, packet.getLength());
	EXPECT_EQ(EncapsStatusCodes::SUCCESS, packet.getStatusCode());
	EXPECT_EQ(0xaabbccdd, packet.getSessionHandle());
	EXPECT_EQ(std::vector<uint8_t >({0, 0, 0, 0,
				  0x64, 0, 1, 2, 3, 4, 5}), packet.getData());
}

TEST(TestEncapsPacket, ShouldThrowErrorIfThePackageTooShort) {
	std::vector<uint8_t> data = {0x6F, 0, 0xB, 0, 0xDD, 0xCC, 0xBB, 0xAA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								 0, 0, 0};

	EncapsPacket packet;
	EXPECT_THROW(packet.expand(data), std::length_error);
}

TEST(TestEncapsPacket, ShouldThrowErrorIfThePackageHasWrongLenghtOfData) {
	std::vector<uint8_t> data = {0x6F, 0, 0xB, 0, 0xDD, 0xCC, 0xBB, 0xAA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								 0, 0, 0, 0, 0, 0, 0, 0,
								 0x64, 0, 1, 2, 3, 4, 5, 10};

	EncapsPacket packet;
	EXPECT_THROW(packet.expand(data), std::length_error);
}