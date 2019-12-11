//
// Created by Aleksey Timin on 12/11/19.
//


#include <gtest/gtest.h>
#include "Mocks.h"
#include "vendor/ra/DPIFaultObject.h"

using namespace eipScanner;
using namespace eipScanner::vendor::ra;

class TestDPIFaultObject : public ::testing::Test {
public:
	const static cip::CipUint OBJECT_ID = 1;
	const std::vector<uint8_t> FULL_INFORMATION_DATA = {
		0x01,			// fault code
		0x02,			// DSI port
		0x03,			// DSI device
		5, 0x0, 'E', 'R', 'R', 'O', 'R',			// fault text
		0x01, 0x02, 0x03, 0x4, 0x5, 0x6, 0x7,0x8,	// timer value
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // flags
	};

	void SetUp() override {
		_messageRouter = std::make_shared<TMockMessageRouter>();
		_nullSession = nullptr;
	}

	TMockMessageRouter::SPtr _messageRouter;
	SessionInfoIf::SPtr  _nullSession;
};

TEST_F(TestDPIFaultObject, ShouldReadAllDataInConstructor) {
	cip::MessageRouterResponse response;
	response.setData(FULL_INFORMATION_DATA);

	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x97, OBJECT_ID),
			std::vector<uint8_t>()
	)).WillOnce(::testing::Return(response));

	DPIFaultObject faultObject(OBJECT_ID, _nullSession, _messageRouter);

	EXPECT_EQ(1, faultObject.getFullInformation().faultCode);
}

