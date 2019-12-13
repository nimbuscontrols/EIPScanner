
#include <gtest/gtest.h>
#include "utils/Buffer.h"

using namespace eipScanner;
using eipScanner::utils::Buffer;

TEST(TestBuffer, PushUint8) {
	Buffer buf;
	std::vector<uint8_t> expectedData = {0, 1, 2};

	buf << expectedData[0] << expectedData[1] << expectedData[2];

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PushInt8) {
	Buffer buf;
	std::vector<uint8_t> expectedData = {0, 1, 2};

	buf << (int8_t)expectedData[0] << (int8_t)expectedData[1] << (int8_t)expectedData[2];

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PushUint16) {
	Buffer buf;
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};

	buf << (uint16_t)0x100 << (uint16_t)0x0302;

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PushInt16) {
	Buffer buf;
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};

	buf << (int16_t)0x100 << (int16_t)0x0302;

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PushUint32) {
	Buffer buf;
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};

	buf << (uint32_t)0x03020100;

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PushInt32) {
	Buffer buf;
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};

	buf << (int32_t)0x03020100;

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PushUint64) {
	Buffer buf;
	std::vector<uint8_t> expectedData = {0, 1, 2, 3, 4, 5, 6, 7};

	buf << (uint64_t)0x0706050403020100;

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PushVectorUint8) {
	Buffer buf;
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};
	std::vector<uint8_t>  v = {0, 1, 2, 3};
	buf << v;

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PushVectorUint16) {
	Buffer buf;
	std::vector<uint16_t> v = {0x100, 0x0302};
	std::vector<uint8_t>  expectedData = {0, 1, 2, 3};
	buf << v;

	EXPECT_EQ(buf.data(), expectedData);
}

TEST(TestBuffer, PullUint8) {
	std::vector<uint8_t> expectedData = {0, 1, 2};
	Buffer buf(expectedData);
	uint8_t a, b, c;

	buf >> a >> b >> c;

	EXPECT_EQ(expectedData[0], a);
	EXPECT_EQ(expectedData[1], b);
	EXPECT_EQ(expectedData[2], c);
}

TEST(TestBuffer, PullInt8) {
	std::vector<uint8_t> expectedData = {0, 1, 2};
	Buffer buf(expectedData);
	int8_t a, b, c;

	buf >> a >> b >> c;

	EXPECT_EQ(expectedData[0], a);
	EXPECT_EQ(expectedData[1], b);
	EXPECT_EQ(expectedData[2], c);
}

TEST(TestBuffer, PullUint16) {
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};
	Buffer buf(expectedData);
	uint16_t a, b;

	buf >> a >> b;

	EXPECT_EQ((uint16_t)0x100,  a);
	EXPECT_EQ((uint16_t)0x0302, b);
}

TEST(TestBuffer, PullInt16) {
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};
	Buffer buf(expectedData);
	int16_t a, b;

	buf >> a >> b;

	EXPECT_EQ((int16_t)0x100,  a);
	EXPECT_EQ((int16_t)0x0302, b);
}

TEST(TestBuffer, PullUint32) {
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};
	Buffer buf(expectedData);
	uint32_t a;

	buf >> a;

	EXPECT_EQ((uint32_t)0x03020100,  a);
}

TEST(TestBuffer, PullInt32) {
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};
	Buffer buf(expectedData);
	int32_t a;

	buf >> a;

	EXPECT_EQ((int32_t)0x03020100,  a);
}

TEST(TestBuffer, PullUint64) {
	std::vector<uint8_t> expectedData = {0, 1, 2, 3, 4, 5, 6, 7};
	Buffer buf(expectedData);
	uint64_t a;

	buf >> a;

	EXPECT_EQ(0x0706050403020100,  a);
}


TEST(TestBuffer, PullVectorUint8) {
	std::vector<uint8_t> expectedData = {0, 1, 2, 3};
	Buffer buf(expectedData);
	std::vector<uint8_t> a(4);

	buf >> a;

	EXPECT_EQ(expectedData,  a);
}

TEST(TestBuffer, PullVectorUint16) {
	std::vector<uint16_t> expectedData = {0x100, 0x0302};
	std::vector<uint8_t> initData = {0, 1, 2, 3};

	Buffer buf(initData);
	std::vector<uint16_t> v(2);
	buf >> v;

	EXPECT_EQ(expectedData, v);
}

TEST(TestBuffer, ShouldDecodeRevision) {
	cip::CipRevision revision(1,2);
	std::vector<uint8_t> expectedData = {1,2};
	Buffer buf;
	buf << revision;

	EXPECT_EQ(expectedData, buf.data());
}

TEST(TestBuffer, ShouldEncodeRevision) {
	cip::CipRevision revision;
	std::vector<uint8_t> data = {1,2};
	Buffer buf(data);
	buf >> revision;

	EXPECT_EQ(revision.getMajorRevision(), 1);
	EXPECT_EQ(revision.getMinorRevision(), 2);
}

TEST(TestBuffer, ShouldDecodeEndPPoint) {
	sockets::EndPoint endPoint("127.0.0.1", 2222);
	std::vector<uint8_t> expectedData = {
			0x00, 0x02,
			0x08, 0xae,
			0x07f, 0x0, 0x0, 0x1,
			0, 0, 0, 0, 0, 0, 0, 0
	};
	Buffer buf;
	buf << endPoint;

	EXPECT_EQ(expectedData, buf.data());
}

TEST(TestBuffer, ShouldEncodeEndPPoint) {
	sockets::EndPoint endPoint("", 0);
	std::vector<uint8_t> data =  {
			0x00, 0x02,
			0x08, 0xae,
			0x07f, 0x0, 0x0, 0x1,
			0, 0, 0, 0, 0, 0, 0, 0
	};
	Buffer buf(data);
	buf >> endPoint;

	EXPECT_EQ("127.0.0.1", endPoint.getHost());
	EXPECT_EQ(2222, endPoint.getPort());
	EXPECT_EQ(2, endPoint.getAddr().sin_family);
	EXPECT_EQ(0x0100007f, endPoint.getAddr().sin_addr.s_addr);
	EXPECT_EQ(0xae08, endPoint.getAddr().sin_port);
}