//
// Created by Aleksey Timin on 12/11/19.
//

#include <gtest/gtest.h>
#include "Mocks.h"
#include "vendor/ra/DPIFaultManager.h"

using namespace eipScanner;
using namespace eipScanner::vendor::ra;

class TestDPIFaultManager : public ::testing::Test {
public:
	const std::vector<uint8_t> FULL_INFORMATION_DATA = {
			0x01,0x0,		// fault code
			0x02,			// DSI port
			0x03,			// DSI device
			'E', 'R', 'R', 'O', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // fault text
			0x01, 0x02, 0x03, 0x4, 0x5, 0x6, 0x7,0x8,	// timer value
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // flags
	};

	void SetUp() override {
		_messageRouter = std::make_shared<TMockMessageRouter>();
		_nullSession = nullptr;

		cip::MessageRouterResponse response;
		response.setData({0});
		EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
												 cip::EPath(0x97, 0, 4))).WillRepeatedly(::testing::Return(response));

		response.setData({0, 0});
		EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
												 cip::EPath(0x97, 0, 6))).WillRepeatedly(::testing::Return(response));
	}

	TMockMessageRouter::SPtr _messageRouter;
	SessionInfoIf::SPtr  _nullSession;
};

TEST_F(TestDPIFaultManager, ShouldGenerateTrippedEvent) {
	DPIFaultManager manager;

	cip::CipUsint  tripped = -1;
	manager.setTrippedDeviceListener([&tripped](bool value) {
		tripped = value;
	});

	cip::MessageRouterResponse response;
	response.setData({1});

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x97, 0, 4))).WillOnce(::testing::Return(response));

	manager.handleFaultObjects(_nullSession, _messageRouter);
	EXPECT_EQ(1, tripped);
}

TEST_F(TestDPIFaultManager, ShouldNotGenerateTrippedEventIfItIsNotChanged) {
	DPIFaultManager manager;

	cip::CipUsint  tripped = -1;
	manager.setTrippedDeviceListener([&tripped](bool value) {
		tripped = value;
	});

	cip::MessageRouterResponse response;
	response.setData({0});

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 4))).WillOnce(::testing::Return(response));

	manager.handleFaultObjects(_nullSession, _messageRouter);
	EXPECT_EQ(0, tripped);

	tripped = 0xff;
	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 4))).WillOnce(::testing::Return(response));

	manager.handleFaultObjects(_nullSession, _messageRouter);
	EXPECT_EQ(0xff, tripped);
}

TEST_F(TestDPIFaultManager, ShouldThrowExceptionIfFailedToReadFaultTrip) {
	DPIFaultManager manager;

	cip::MessageRouterResponse response;
	response.setGeneralStatusCode(cip::GeneralStatusCodes::ATTRIBUTE_NOT_SUPPORTED);

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 4))).WillOnce(::testing::Return(response));

	EXPECT_THROW(manager.handleFaultObjects(_nullSession, _messageRouter),
			std::runtime_error);

}

TEST_F(TestDPIFaultManager, ShouldWriteCommand) {
	DPIFaultManager manager;
	cip::MessageRouterResponse response;

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::SET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 3),
											 std::vector<uint8_t >{2})).WillOnce(::testing::Return(response));

	manager.writeCommand(DPIFaultManagerCommands::CLEAR_FAULT_QUEUE, _nullSession, _messageRouter);
}

TEST_F(TestDPIFaultManager, ShouldGenerateEventForEachNewFaultAndWhenCleanTheQueue) {
	DPIFaultManager manager;

	std::vector<DPIFaultObject> faultObjects;
	manager.setNewFaultListener([&faultObjects](const DPIFaultObject& object) {
		faultObjects.push_back(object);
	});

	cip::MessageRouterResponse response;
	response.setData({2, 0});

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 6))).WillOnce(::testing::Return(response));

	response.setData(FULL_INFORMATION_DATA);

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 1, 0))).WillOnce(::testing::Return(response));

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 2, 0))).WillOnce(::testing::Return(response));

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::SET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 3),
											 std::vector<uint8_t >{2})).WillOnce(::testing::Return(response));

	manager.handleFaultObjects(_nullSession, _messageRouter);
	EXPECT_EQ(2, faultObjects.size());
	EXPECT_EQ(1, faultObjects[0].getFullInformation().faultCode);
	EXPECT_EQ(1, faultObjects[1].getFullInformation().faultCode);
}

TEST_F(TestDPIFaultManager, ShouldThrowExceptionIfFailedToReadNumberOfFaults) {
	DPIFaultManager manager;

	cip::MessageRouterResponse response;
	response.setGeneralStatusCode(cip::GeneralStatusCodes::ATTRIBUTE_NOT_SUPPORTED);

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 6))).WillOnce(::testing::Return(response));

	EXPECT_THROW(manager.handleFaultObjects(_nullSession, _messageRouter),
				 std::runtime_error);
}
