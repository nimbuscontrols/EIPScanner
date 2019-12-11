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
		0x01,0x0,		// fault code
		0x02,			// DSI port
		0x03,			// DSI device
		'E', 'R', 'R', 'O', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // fault text
		0x01, 0x02, 0x03, 0x4, 0x5, 0x6, 0x7,0x8,	// timer value
		0x3, 0x0
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
			cip::EPath(0x97, OBJECT_ID, 0)
	)).WillOnce(::testing::Return(response));

	DPIFaultObject faultObject(OBJECT_ID, _nullSession, _messageRouter);

	const auto& fullInformation = faultObject.getFullInformation();
	EXPECT_EQ(1, fullInformation.faultCode);
	EXPECT_EQ(2, fullInformation.dsiPort);
	EXPECT_EQ(3, fullInformation.dsiDeviceObject);
	EXPECT_EQ("ERROR           ", fullInformation.faultText.toStdString());
	EXPECT_EQ(0x0807060504030201, fullInformation.timerValue);
	EXPECT_TRUE(fullInformation.isValidData);
	EXPECT_TRUE(fullInformation.isRealTime);
}

TEST_F(TestDPIFaultObject, ShouldThrowExecptionIfFailedToGetData) {
	cip::MessageRouterResponse response;
	response.setData(FULL_INFORMATION_DATA);
	response.setGeneralStatusCode(cip::GeneralStatusCodes::INVALID_ATTRIBUTE_VALUE);

	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x97, OBJECT_ID, 0)
	)).WillOnce(::testing::Return(response));

	EXPECT_THROW(DPIFaultObject(OBJECT_ID, _nullSession, _messageRouter), std::runtime_error);
}