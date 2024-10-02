//
// Created by Aleksey Timin on 12/4/19.
//

#include <gtest/gtest.h>
#include "EIPScanner/cip/EPath.h"

using eipScanner::cip::EPath;

TEST(TestEPath, ShouldExpandPaddedPath8b) {
	std::vector<uint8_t> data = {0x20, 0x05, 0x24, 0x02, 0x30, 0x01};
	EPath path;
	path.expandPaddedPath(data);

	EXPECT_EQ(EPath(0x05, 0x02, 0x01), path);
}

TEST(TestEPath, ShouldExpandPaddedPath16b) {
	std::vector<uint8_t> data = {0x21, 0x00,  0x05, 0x00, 0x25, 0x00, 0x02, 0x00, 0x31, 0x00, 0x01, 0x00};
	EPath path;
	path.expandPaddedPath(data);

	EXPECT_EQ(EPath(0x05, 0x02, 0x01), path);
}

TEST(TestEPath, ShouldExpandPaddedPathCombo) {
	std::vector<uint8_t> data = {0x21, 0x00,  0x05, 0x00, 0x24, 0x02, 0x31, 0x00, 0x01, 0x00};
	EPath path;
	path.expandPaddedPath(data);

	EXPECT_EQ(EPath(0x05, 0x02, 0x01), path);
}

TEST(TestEPath, ShouldExpandPaddedPathOnlyClass) {
	std::vector<uint8_t> data = {0x21, 0x00,  0x05, 0x00};
	EPath path;
	path.expandPaddedPath(data);

	EXPECT_EQ(EPath(0x05), path);
}

TEST(TestEPath, ShouldExpandPaddedPathOnlyClassAndObject) {
	std::vector<uint8_t> data = {0x21, 0x00,  0x05, 0x00,  0x24, 0x02};
	EPath path;
	path.expandPaddedPath(data);

	EXPECT_EQ(EPath(0x05, 0x02), path);
}

TEST(TestEPath, ShouldThrowExceptionIfThePathHasWrongSegment) {
	std::vector<uint8_t> data = {0x21, 0x00,  0x05, 0x00,  0xf4, 0x02};
	EPath path;

	EXPECT_THROW(path.expandPaddedPath(data), std::runtime_error);
}

TEST(TestEPath, ShouldThrowExceptionIfThePathNotComplited) {
	std::vector<uint8_t> data = {0x21, 0x00,  0x05, 0x00,  0x24};
	EPath path;

	EXPECT_THROW(path.expandPaddedPath(data), std::runtime_error);
}