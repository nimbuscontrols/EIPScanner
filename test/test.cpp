//
// Created by Aleksey Timin on 11/16/19.
//

#include <gtest/gtest.h>

#include "utils/Logger.h"

int main(int argc, char **argv) {
	eipScanner::utils::Logger::setLogLevel(eipScanner::utils::LogLevel::DEBUG);

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
