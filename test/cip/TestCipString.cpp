
//
// Created by Aleksey Timin on 12/4/19.
//

#include <gtest/gtest.h>
#include "cip/CipString.h"
#include "utils/Buffer.h"

using eipScanner::cip::CipBaseString;
using eipScanner::utils::Buffer;

TEST(TestCipString, ShouldConvertToStdString) {
	std::string std_string("Hello!");

	CipBaseString<uint8_t> cipString(std_string);

	EXPECT_EQ(std_string, cipString.toStdString());
}

TEST(TestCipString, ShouldDecodeAndEncodeByBuffer) {
	CipBaseString<uint8_t> srcString("Hello!");
	CipBaseString<uint8_t> dstString;

	Buffer buffer;
	buffer << srcString >> dstString;


	EXPECT_EQ(dstString.toStdString(), srcString.toStdString());
}