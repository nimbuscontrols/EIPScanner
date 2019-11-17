//
// Created by Aleksey Timin on 11/17/19.
//

#include <gtest/gtest.h>
#include "cip/MessageRouterResponse.h"

using eipScanner::cip::MessageRouterResponse;

TEST(TestMessageRouterResponse, ShouldThrowErroIfDataTooShort) {
	std::vector<uint8_t> data = {0, 0, 0};

	MessageRouterResponse response;
	EXPECT_THROW(response.expand(data), std::runtime_error);
}


TEST(TestMessageRouterResponse, ShouldThrowErroIfAdditionalStatusHasWrongSize) {
	std::vector<uint8_t> data = {0, 0, 0, 2, 0, 1};

	MessageRouterResponse response;
	EXPECT_THROW(response.expand(data), std::runtime_error);
}
