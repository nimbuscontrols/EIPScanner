//
// Created by Aleksey Timin on 12/10/19.
//

#include <gtest/gtest.h>

#include "sockets/EndPoint.h"

using eipScanner::sockets::EndPoint;

TEST(TestEndPoint, ShouldConvertIpAndHostToAddr) {
	EndPoint endPoint("127.0.0.1", 0xAA00);

	EXPECT_EQ("127.0.0.1", endPoint.getHost());
	EXPECT_EQ(0xAA00, endPoint.getPort());
	EXPECT_EQ(2, endPoint.getAddr().sin_family);
	EXPECT_EQ(0x0100007F, endPoint.getAddr().sin_addr.s_addr);
	EXPECT_EQ(0x00AA, endPoint.getAddr().sin_port);
	EXPECT_EQ("127.0.0.1:43520", endPoint.toString());
}

TEST(TestEndPoint, ShouldConvertAddrToIpAndHost) {
	struct sockaddr_in addr{0};
	addr.sin_family = 2;
	addr.sin_addr.s_addr = 0x0100007F;
	addr.sin_port = 0x00AA;

	EndPoint endPoint(addr);

	EXPECT_EQ("127.0.0.1", endPoint.getHost());
	EXPECT_EQ(0xAA00, endPoint.getPort());
	EXPECT_EQ(2, endPoint.getAddr().sin_family);
	EXPECT_EQ(0x0100007F, endPoint.getAddr().sin_addr.s_addr);
	EXPECT_EQ(0x00AA, endPoint.getAddr().sin_port);
	EXPECT_EQ("127.0.0.1:43520", endPoint.toString());
}

TEST(TestEndPoint, ShouldNotThrowExceptionIfCanNotParseIP) {
	EndPoint endPoint("XXXX", 0);
	EXPECT_EQ(0, endPoint.getAddr().sin_addr.s_addr);
}
