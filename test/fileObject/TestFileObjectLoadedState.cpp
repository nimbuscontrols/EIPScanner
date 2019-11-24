//
// Created by Aleksey Timin on 11/24/19.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Mocks.h"
#include "FileObject.h"
#include "fileObject/FileObjectLoadedState.h"
#include "utils/Buffer.h"

using namespace eipScanner;
using eipScanner::fileObject::FileObjectLoadedState;
using eipScanner::fileObject::FILE_OBJECT_CLASS_ID;
using utils::Buffer;

using ::testing::Return;

class TestFileObjectLoadedState : public ::testing::Test {
public:
	const cip::CipUint FILE_OBJECT_ID = 1;

	void SetUp() override {
		_messageRouter = std::make_shared<fileObject::MockMessageRouter>();

		mockGetFileObjectState(_messageRouter, _nullSession, FILE_OBJECT_ID);

		_fileObject = std::make_unique<FileObject>(FILE_OBJECT_ID, _nullSession, _messageRouter);
	}

	fileObject::MockMessageRouter::SPtr _messageRouter;
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