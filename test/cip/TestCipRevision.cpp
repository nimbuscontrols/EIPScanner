//
// Created by Aleksey Timin on 12/8/19.
//

#include <gtest/gtest.h>
#include "EIPScanner/cip/CipRevision.h"

using eipScanner::cip::CipRevision;

TEST(TestCipRevision, ShouldHaveConstructors) {
	CipRevision revision;

	EXPECT_EQ(CipRevision(0,0), revision);
}

TEST(TestCipRevision, ShouldConvertToString) {
	CipRevision revision(1,2);

	EXPECT_EQ(1, revision.getMajorRevision());
	EXPECT_EQ(2, revision.getMinorRevision());
	EXPECT_EQ("1.2", revision.toString());
}