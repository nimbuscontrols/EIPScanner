//
// Created by Aleksey Timin on 11/16/19.
//
#include <gtest/gtest.h>
#include "eip/EncapsPacketFactory.h"
#include "eip/EncapsPacket.h"

using eipScanner::eip::EncapsPacket;
using eipScanner::eip::EncapsPacketFactory;

TEST(TestEncapsPacketFactory, ShouldCreateRegisterSessionPacket) {
	std::vector<uint8_t> expectedPacket = {0x65, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
											1, 0, 0, 0};

	EncapsPacket packet = EncapsPacketFactory().createRegisterSessionPacket();
	EXPECT_EQ(expectedPacket, packet.pack());
}


TEST(TestEncapsPacketFactory, ShouldCreateUnRegisterSessionPacket) {
	std::vector<uint8_t> expectedPacket = {0x66, 0, 0, 0, 0xdd, 0xcc, 0xbb, 0xaa, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
											0, 0, 0, 0};

	EncapsPacket packet = EncapsPacketFactory().createUnRegisterSessionPacket(0xaabbccdd);
	EXPECT_EQ(expectedPacket, packet.pack());
}

TEST(TestEncapsPacketFactory, ShouldCreateSendRRDataPacket) {
	std::vector<uint8_t> data = {1, 2, 3, 4, 5};
	std::vector<uint8_t> expectedPacket = {0x6F, 0, 0xB, 0, 0xDD, 0xCC, 0xBB, 0xAA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
											0, 0, 0, 0, 0, 0, 0, 0,
											0x64, 0, 1, 2, 3, 4, 5};

	EncapsPacket packet = EncapsPacketFactory().createSendRRDataPacket(0xaabbccdd, 100, data);
	EXPECT_EQ(expectedPacket, packet.pack());
}

TEST(TestEncapsPacketFactory, ShouldCreateListIdentityPacket) {
	std::vector<uint8_t> expectedPacket = {0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										   0, 0, 0, 0};

	EncapsPacket packet = EncapsPacketFactory().createListIdentityPacket();
	EXPECT_EQ(expectedPacket, packet.pack());
}