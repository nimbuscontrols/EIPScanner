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
			0x0, 0x0,             // descriptor
			(uint8_t)cip::CipDataTypes::UDINT, // type
			0x4,                  // data size
			5, 'P', 'A', 'R', 'A', 'M', //name
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

	EXPECT_EQ(0x1, parameterObject.getValue<cip::CipUdint>());
	EXPECT_EQ(cip::CipDataTypes::UDINT, parameterObject.getType());
	EXPECT_EQ("", parameterObject.getName());
}

TEST_F(TestParameterObject, ShouldReadAllFullDataInConstructor) {
	mockReadingParamData();
	ParameterObject parameterObject(OBJECT_ID, true, _nullSession, _messageRouter);

	EXPECT_EQ(0x1, parameterObject.getValue<cip::CipUdint>());
	EXPECT_EQ(cip::CipDataTypes::UDINT, parameterObject.getType());
	EXPECT_EQ("PARAM", parameterObject.getName());
}