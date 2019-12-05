//
// Created by Aleksey Timin on 12/4/19.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ParameterObject.h"


using namespace eipScanner;

class TMockMessageRouter : public MessageRouter {
public:
	using SPtr = std::shared_ptr<TMockMessageRouter>;

	MOCK_CONST_METHOD4(sendRequest, cip::MessageRouterResponse(
			SessionInfo::SPtr si,
			cip::CipUsint service,
			const cip::EPath &path,
			const std::vector<uint8_t> &data));
};

using ::testing::Return;

class TestParameterObject : public ::testing::Test {
public:
	const static cip::CipUint OBJECT_ID = 1;
	const std::vector<uint8_t> PARAM_DATA = {
			0x01, 0x0, 0x0, 0x0,  //value
			0x6,                  //path size
			0x20, 0x05, 0x24, 0x02, 0x30, 0x01,  // path
			0x4, 0x0,             // descriptor (scalable)
			(uint8_t)cip::CipDataTypes::UDINT, // type
			0x4,                  // data size
			5, 'P', 'A', 'R', 'A', 'M', //name
			3, 'M', 'P', 'a',
			4, 'H', 'E', 'L', 'P',
			0x0, 0x0, 0x0, 0x0,    //min value
			0x5, 0x0, 0x0, 0x0,    //max value
			0x3, 0x0, 0x0, 0x0,    //default value
			0x2, 0x0,              // scaling mult
			0x4, 0x0,              // scaling div
			0x1, 0x0,              // scaling base
			0x6, 0x0,              // scaling offset
	};

	void SetUp() override {
		_messageRouter = std::make_shared<TMockMessageRouter>();
		_nullSession = nullptr;
	}

	void mockReadingParamData() {
		cip::MessageRouterResponse response;
		response.setData({4});

		EXPECT_CALL(*_messageRouter, sendRequest(_nullSession,
												 cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
												 cip::EPath(ParameterObject::CLASS_ID, OBJECT_ID, 6), std::vector<uint8_t>())).WillOnce(Return(response));

		response.setData(PARAM_DATA);

		EXPECT_CALL(*_messageRouter, sendRequest(_nullSession,
												 cip::ServiceCodes::GET_ATTRIBUTE_ALL,
												 cip::EPath(ParameterObject::CLASS_ID, OBJECT_ID), std::vector<uint8_t>())).WillOnce(Return(response));
	}
	TMockMessageRouter::SPtr _messageRouter;
	SessionInfo::SPtr  _nullSession;
};

TEST_F(TestParameterObject, ShouldReadAllStubDataInConstructor) {
	mockReadingParamData();
	ParameterObject parameterObject(OBJECT_ID, false, _nullSession, _messageRouter);

	EXPECT_FALSE(parameterObject.hasFullAttributes());
	EXPECT_FALSE(parameterObject.isScalable());
	EXPECT_EQ(0x1, parameterObject.getActualValue<cip::CipUdint>());
	EXPECT_EQ(cip::CipDataTypes::UDINT, parameterObject.getType());
	EXPECT_EQ("", parameterObject.getName());
}

TEST_F(TestParameterObject, ShouldReadAllFullDataInConstructor) {
	mockReadingParamData();
	ParameterObject parameterObject(OBJECT_ID, true, _nullSession, _messageRouter);

	EXPECT_TRUE(parameterObject.hasFullAttributes());
	EXPECT_EQ(0x1, parameterObject.getActualValue<cip::CipUdint>());
	EXPECT_EQ(cip::CipDataTypes::UDINT, parameterObject.getType());
	EXPECT_EQ("PARAM", parameterObject.getName());
	EXPECT_EQ("MPa", parameterObject.getUnits());
	EXPECT_EQ("HELP", parameterObject.getHelp());
	EXPECT_EQ(0, parameterObject.getMinValue<cip::CipUdint>());
	EXPECT_EQ(5, parameterObject.getMaxValue<cip::CipUdint>());
	EXPECT_EQ(3, parameterObject.getDefaultValue<cip::CipUdint>());

	// Scaling
	EXPECT_TRUE(parameterObject.isScalable());
	EXPECT_EQ(2, parameterObject.getScalingMultiplier());
	EXPECT_EQ(4, parameterObject.getScalingDivisor());
	EXPECT_EQ(1, parameterObject.getScalingBase());
	EXPECT_EQ(6, parameterObject.getScalingOffset());

	EXPECT_FLOAT_EQ(3.5, parameterObject.getEngValue<cip::CipUdint>());
}

TEST_F(TestParameterObject, ShouldUpdateValue) {
	mockReadingParamData();
	ParameterObject parameterObject(OBJECT_ID, true, _nullSession, _messageRouter);

	EXPECT_EQ(0x1, parameterObject.getActualValue<cip::CipUdint>());

	cip::MessageRouterResponse response;
	response.setData({4, 0, 0, 0});

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession,
											 cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(ParameterObject::CLASS_ID, OBJECT_ID, 1), std::vector<uint8_t>())).WillOnce(Return(response));

	parameterObject.updateValue(_nullSession);
	EXPECT_EQ(0x4, parameterObject.getActualValue<cip::CipUdint>());
}