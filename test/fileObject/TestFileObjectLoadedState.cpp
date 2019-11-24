//
// Created by Aleksey Timin on 11/24/19.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MessageRouter.h"
#include "FileObject.h"
#include "fileObject/FileObjectLoadedState.h"
#include "utils/Buffer.h"

using namespace eipScanner;
using fileObject::FileObjectLoadedState;
using fileObject::FileObjectAttributesCodes;
using fileObject::FILE_OBJECT_CLASS_ID;
using utils::Buffer;

using ::testing::Return;

class MockMessageRouter : public MessageRouter {
public:
	using SPtr = std::shared_ptr<MockMessageRouter>;
	MOCK_CONST_METHOD4(sendRequest, cip::MessageRouterResponse(SessionInfo::SPtr si, cip::CipUsint service,
										   const cip::EPath& path, const std::vector<uint8_t>& data));
};

class TestFileObjectLoadedState : public ::testing::Test {
public:
	const cip::CipUint FILE_OBJECT_ID = 1;

	void SetUp() override {
		_messageRouter = std::make_shared<MockMessageRouter>();

		cip::MessageRouterResponse response;
		Buffer buffer;
		buffer << static_cast<cip::CipUsint>(FileObjectStateCodes::FILE_LOADED);

		response.setData(buffer.data());
		response.setGeneralStatusCode(cip::GeneralStatusCodes::SUCCESS);

		EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
				cip::EPath(FILE_OBJECT_CLASS_ID, FILE_OBJECT_ID,
				static_cast<cip::CipUint>(FileObjectAttributesCodes::STATE)),
				std::vector<uint8_t>())
				).WillOnce(Return(response));

		_fileObject = std::make_unique<FileObject>(FILE_OBJECT_ID, _nullSession, _messageRouter);
	}

	MockMessageRouter::SPtr _messageRouter;
	FileObject::UPtr _fileObject;
	SessionInfo::SPtr _nullSession;

};

TEST_F(TestFileObjectLoadedState, ShouldInitiateUpload) {

	cip::MessageRouterResponse response;
	Buffer buffer;
	const cip::CipUdint FILE_SIZE = 1000000;
	const cip::CipUsint TRANSFER_SIZE = 0xff;
	buffer << FILE_SIZE << TRANSFER_SIZE;

	response.setData(buffer.data());
	response.setGeneralStatusCode(cip::GeneralStatusCodes::SUCCESS);

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, 0x4b,
											 cip::EPath(FILE_OBJECT_CLASS_ID, FILE_OBJECT_ID),
											 std::vector<uint8_t>{0xff})
	).WillOnce(Return(response));

	_fileObject->beginUpload(nullptr, [](auto status, auto fileContent){});

	EXPECT_EQ(FileObjectStateCodes::TRANSFER_UPLOAD_IN_PROGRESS, _fileObject->getState());
}