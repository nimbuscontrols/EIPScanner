//
// Created by Aleksey Timin on 12/11/19.
//


#include <gtest/gtest.h>
#include "Mocks.h"
#include "EIPScanner/vendor/ra/powerFlex525/DPIFaultObject.h"
#include "EIPScanner/vendor/ra/powerFlex525/DPIFaultParameter.h"

using namespace eipScanner;
using namespace eipScanner::vendor::ra::powerFlex525;

class TestDPIParameterObject : public ::testing::Test {
public:
	void SetUp() override {
		_messageRouter = std::make_shared<TMockMessageRouter>();
		_nullSession = nullptr;
	}

	TMockMessageRouter::SPtr _messageRouter;
	SessionInfoIf::SPtr  _nullSession;
};

TEST_F(TestDPIParameterObject, ShouldReadAllDataInConstructor) {
	cip::MessageRouterResponse response;
	response.setData({4,0});

	// fault code
	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 7, 1)
	)).WillOnce(::testing::Return(response));

	// volts
	response.setData({3,0});
	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 651, 1)
	)).WillOnce(::testing::Return(response));

	// current
	response.setData({200,0});
	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 641, 1)
	)).WillOnce(::testing::Return(response));

	// freq
	response.setData({100,0});
	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 631, 1)
	)).WillOnce(::testing::Return(response));

	DPIFaultParameter faultParameter(_nullSession, _messageRouter, 1, true);

	auto fullInformation = faultParameter.getFullInformation();
	EXPECT_EQ(1, fullInformation.faultDetails.frequency);
	EXPECT_EQ(2, fullInformation.faultDetails.current);
	EXPECT_EQ(3, fullInformation.faultDetails.busVoltage);
	EXPECT_EQ(4, fullInformation.faultDetails.faultCode);
	EXPECT_EQ(1, fullInformation.faultDetails.faultNumber);
}

TEST_F(TestDPIParameterObject, ShouldThrowExecptionIfFailedToGetData) {
	cip::MessageRouterResponse response;
	response.setGeneralStatusCode(cip::GeneralStatusCodes::INVALID_ATTRIBUTE_VALUE);

	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 7, 1)
	)).WillOnce(::testing::Return(response));

	EXPECT_THROW(DPIFaultParameter(_nullSession, _messageRouter, 1, false), std::runtime_error);
}