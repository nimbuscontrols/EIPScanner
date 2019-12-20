//
// Created by Aleksey Timin on 12/11/19.
//


#include <gtest/gtest.h>
#include "Mocks.h"
#include "vendor/ra/powerFlex525/DPIFaultObject.h"
#include "vendor/ra/powerFlex525/DPIFaultParameter.h"

using namespace eipScanner;
using namespace eipScanner::vendor::ra::powerFlex525;

class TestDPIFaultObject : public ::testing::Test {
public:
	const static cip::CipUint OBJECT_ID = 1;
	const std::vector<uint8_t> FULL_INFORMATION_DATA = {
		0x01,		// frequency
		0x02,		// current
		0x03,		// bus voltage
		0x04,       // fault code
		0x05,       // fault number
		0x06,       // fault type
		'U', 'N', 'D', 'E', 'R', 'V', 'O', 'L', 'T', 'A', 'G', 'E',  // fault text,
		'E', 'R', 'R', 'O', 'R', ' ',  // fault description
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

	DPIFaultParameter faultParameter(_nullSession, _messageRouter, 1, true);

	const auto& fullInformation = faultParameter.getFullInformation();
	EXPECT_EQ(1, fullInformation.faultDetails.frequency);
	EXPECT_EQ(2, fullInformation.faultDetails.current);
	EXPECT_EQ(3, fullInformation.faultDetails.busVoltage);
	EXPECT_EQ(4, fullInformation.faultDetails.faultCode);
	EXPECT_EQ(5, fullInformation.faultDetails.faultNumber);
	EXPECT_EQ(6, fullInformation.faultDescription.faultType);
	EXPECT_EQ("UNDERVOLTAGE", fullInformation.faultDescription.faultText);
	EXPECT_EQ("ERROR ", fullInformation.faultDescription.faultDescription);
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

	EXPECT_THROW(DPIFaultParameter(_nullSession, _messageRouter, 1, false), std::runtime_error);
}