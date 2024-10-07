//
// Created by Aleksey Timin on 11/24/19.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FileObjectMocks.h"
#include "EIPScanner/FileObject.h"
#include "EIPScanner/fileObject/FileObjectUploadInProgressState.h"
#include "EIPScanner/utils/Buffer.h"

using namespace eipScanner;
using eipScanner::fileObject::FileObjectUploadInProgressState;
using eipScanner::fileObject::FILE_OBJECT_CLASS_ID;
using eipScanner::fileObject::TransferPacketTypeCodes;
using utils::Buffer;

using ::testing::Return;

class TestFileObjectUploadInProgressState : public ::testing::Test {
public:
	const cip::CipUint FILE_OBJECT_ID = 1;
	const int FILE_SIZE = 255*2+10;

	void SetUp() override {
		_messageRouter = std::make_shared<TMockMessageRouter>();
		fileObject::mockGetFileObjectState(_messageRouter, _nullSession,
				FILE_OBJECT_ID, FileObjectStateCodes::TRANSFER_UPLOAD_IN_PROGRESS);

		_fileObject = std::make_unique<FileObject>(FILE_OBJECT_ID, _nullSession, _messageRouter);

		_fileObject->getState().reset(new fileObject::FileObjectUploadInProgressState(
				*_fileObject,
				FILE_OBJECT_ID,
				_messageRouter,
				FILE_SIZE,
				255
				,[this](auto status, auto data) {
					_receivedStatus = status;
					_receivedFile = data;
				}));
	}

	void mockTransfer(cip::CipUsint transferNumber, TransferPacketTypeCodes type, std::vector<uint8_t> fileData) {
		auto transferType = static_cast<cip::CipUsint>(type);

		Buffer buffer;
		buffer << transferNumber << transferType;

		if (type == TransferPacketTypeCodes::LAST || type == TransferPacketTypeCodes::FIRST_AND_LAST) {
			cip::CipInt crc = 0;
			buffer << crc;
		}

		buffer << fileData;

		cip::MessageRouterResponse response;
		response.setData(buffer.data());

		EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, 0x4f,
												 cip::EPath(FILE_OBJECT_CLASS_ID, FILE_OBJECT_ID),
												 std::vector<uint8_t>{transferNumber})
		).WillOnce(Return(response));
	}

	TMockMessageRouter::SPtr _messageRouter;
	FileObject::UPtr _fileObject;
	SessionInfoIf::SPtr _nullSession;

	cip::GeneralStatusCodes _receivedStatus;
	std::vector<uint8_t> _receivedFile;
};

TEST_F(TestFileObjectUploadInProgressState, ShouldUploadTransfer) {
	mockTransfer(0, TransferPacketTypeCodes::FIRST, std::vector<uint8_t>(255));
	EXPECT_TRUE(_fileObject->handleTransfers(_nullSession));

	mockTransfer(1, TransferPacketTypeCodes::MIDDLE, std::vector<uint8_t>(255));
	EXPECT_TRUE(_fileObject->handleTransfers(_nullSession));

	mockTransfer(2, TransferPacketTypeCodes::LAST, std::vector<uint8_t>(10));
	EXPECT_FALSE(_fileObject->handleTransfers(_nullSession));

	EXPECT_EQ(_receivedStatus, cip::GeneralStatusCodes::SUCCESS);
	EXPECT_EQ(_receivedFile, std::vector<uint8_t>(FILE_SIZE));
}

TEST_F(TestFileObjectUploadInProgressState, ShouldUploadTransferFirstAndLastPackage) {
	_fileObject->getState().reset(new fileObject::FileObjectUploadInProgressState(
			*_fileObject,
			FILE_OBJECT_ID,
			_messageRouter,
			56,
			255
			,[this](auto status, auto data) {
				_receivedStatus = status;
				_receivedFile = data;
			}));

	mockTransfer(0, TransferPacketTypeCodes::FIRST_AND_LAST, std::vector<uint8_t>(56));
	EXPECT_FALSE(_fileObject->handleTransfers(_nullSession));


	EXPECT_EQ(_receivedStatus, cip::GeneralStatusCodes::SUCCESS);
	EXPECT_EQ(_receivedFile, std::vector<uint8_t>(56));
}

TEST_F(TestFileObjectUploadInProgressState, ShouldUploadTransferAndCheckSize) {

	mockTransfer(0, TransferPacketTypeCodes::FIRST, std::vector<uint8_t>(255));
	EXPECT_TRUE(_fileObject->handleTransfers(_nullSession));

	mockTransfer(1, TransferPacketTypeCodes::LAST, std::vector<uint8_t>(12));
	EXPECT_FALSE(_fileObject->handleTransfers(_nullSession));

	EXPECT_EQ(_receivedStatus, cip::GeneralStatusCodes::INVALID_PARAMETER);
	EXPECT_EQ(_receivedFile, std::vector<uint8_t>());
}

TEST_F(TestFileObjectUploadInProgressState, ShouldUploadTransferAndCheckTransferNumber) {
	cip::CipUsint transferNumber = 255;
	auto transferType = static_cast<cip::CipUsint>(TransferPacketTypeCodes::FIRST);

	Buffer buffer;
	buffer << transferNumber << transferType;
	buffer << std::vector<uint8_t>(255);

	cip::MessageRouterResponse response;
	response.setData(buffer.data());


	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, 0x4f,
											 cip::EPath(FILE_OBJECT_CLASS_ID, FILE_OBJECT_ID),
											 std::vector<uint8_t>{0})
	).WillOnce(Return(response));

	EXPECT_FALSE(_fileObject->handleTransfers(_nullSession));
	EXPECT_EQ(_receivedStatus, cip::GeneralStatusCodes::INVALID_PARAMETER);
	EXPECT_EQ(_receivedFile, std::vector<uint8_t>());
}
