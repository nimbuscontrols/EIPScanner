//
// Created by Aleksey Timin on 12/9/19.
//

#include <gtest/gtest.h>

#include "Mocks.h"
#include "IdentityObject.h"

using namespace eipScanner;

class TestIdentityObject : public ::testing::Test {
public:
	const static cip::CipUint OBJECT_ID = 1;
	const std::vector<uint8_t> IDENTITY_DATA = {
			0x01, 0x0,			// vandor ID
			0x02, 0x0,			// device type
			0x03, 0x0,			// product code
			0x03, 0x04,			// revision
			0x05, 0x06,			//status
			0x07, 0x08, 0x09, 0x0A, // serial number
			5, 'D', 'E', 'V', '0', '1', //product name

	};

	void SetUp() override {
		_messageRouter = std::make_shared<TMockMessageRouter>();
		_nullSession = nullptr;
	}

	TMockMessageRouter::SPtr _messageRouter;
	SessionInfoIf::SPtr  _nullSession;
};

TEST_F(TestIdentityObject, ShouldReadAllDataInConstructor) {
	cip::MessageRouterResponse response;
	response.setData(IDENTITY_DATA);

	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_ALL,
			cip::EPath(1,OBJECT_ID),
			std::vector<uint8_t>()
			)).WillOnce(::testing::Return(response));

	IdentityObject identityObject(OBJECT_ID, _nullSession, _messageRouter);

	EXPECT_EQ(1, identityObject.getVendorId());
	EXPECT_EQ(2, identityObject.getDeviceType());
	EXPECT_EQ(3, identityObject.getProductCode());
	EXPECT_EQ(cip::CipRevision(3,4), identityObject.getRevision());
	EXPECT_EQ(0x605, identityObject.getStatus());
	EXPECT_EQ(0xA090807, identityObject.getSerialNumber());
	EXPECT_EQ("DEV01", identityObject.getProductName());
}
