//
// Created by Aleksey Timin on 11/24/19.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FileObjectMocks.h"
#include "EIPScanner/FileObject.h"
#include "EIPScanner/fileObject/FileObjectLoadedState.h"
#include "EIPScanner/utils/Buffer.h"

using namespace eipScanner;
using eipScanner::fileObject::FileObjectLoadedState;
using eipScanner::fileObject::FILE_OBJECT_CLASS_ID;
using utils::Buffer;

using ::testing::Return;

class TestFileObjectLoadedState : public ::testing::Test {
public:
	const cip::CipUint FILE_OBJECT_ID = 1;

	void SetUp() override {
		_messageRouter = std::make_shared<TMockMessageRouter>();
		fileObject::mockGetFileObjectState(_messageRouter, _nullSession,
				FILE_OBJECT_ID, FileObjectStateCodes::FILE_LOADED);

		_fileObject = std::make_unique<FileObject>(FILE_OBJECT_ID, _nullSession, _messageRouter);
	}

	static cip::MessageRouterResponse makeResponse() {
		cip::MessageRouterResponse response;
		Buffer buffer;
		const cip::CipUdint FILE_SIZE = 1000000;
		const cip::CipUsint TRANSFER_SIZE = 0xff;
		buffer << FILE_SIZE << TRANSFER_SIZE;

		response.setData(buffer.data());
		response.setGeneralStatusCode(cip::SUCCESS);
		return response;
	}

	TMockMessageRouter::SPtr _messageRouter;
	FileObject::UPtr _fileObject;
	SessionInfoIf::SPtr _nullSession;
};

TEST_F(TestFileObjectLoadedState, ShouldInitiateUpload) {
	cip::MessageRouterResponse response = makeResponse();

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, 0x4b,
											 cip::EPath(FILE_OBJECT_CLASS_ID, FILE_OBJECT_ID),
											 std::vector<uint8_t>{0xff})
	).WillOnce(Return(response));

	_fileObject->beginUpload(nullptr, [](auto status, auto fileContent){});
	EXPECT_EQ(FileObjectStateCodes::TRANSFER_UPLOAD_IN_PROGRESS, _fileObject->getState()->getStateCode());
}

TEST_F(TestFileObjectLoadedState, ShouldCallHandlerIfItFailedToInitiateUpload) {
	cip::MessageRouterResponse response = makeResponse();
	response.setGeneralStatusCode(cip::GeneralStatusCodes::INVALID_PARAMETER);

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, 0x4b,
											 cip::EPath(FILE_OBJECT_CLASS_ID, FILE_OBJECT_ID),
											 std::vector<uint8_t>{0xff})
		).WillOnce(Return(response));

	auto receivedStatus = cip::GeneralStatusCodes::SUCCESS;
	_fileObject->beginUpload(nullptr, [&receivedStatus](auto status, auto fileContent){
		receivedStatus = status;
	});

	EXPECT_EQ(cip::GeneralStatusCodes::INVALID_PARAMETER, receivedStatus);
}