//
// Created by Aleksey Timin on 11/23/19.
//

#include "src/FileObjectUploadInProgressState.h"
#include "src/utils/Buffer.h"

namespace eipScanner {
namespace fileObject {
	using utils::LogLevel;
	using utils::Buffer;

	FileObjectUploadInProgressState::FileObjectUploadInProgressState(FileObject &owner, cip::CipUint objectId,
																	 MessageRouter::SPtr messageRouter,
																	 cip::CipUdint fileSize,
																	 cip::CipUsint transferSize,
																	 EndUploadHandler handler)
			: FileObjectState{FileObjectStateCodes::TRANSFER_UPLOAD_IN_PROGRESS, owner, objectId, messageRouter}
			, _fileSize{fileSize}
			, _transferSize{transferSize}
			, _fileContent()
			, _transferNumber{0}
			, _handler(std::move(handler)){
	}

	void
	FileObjectUploadInProgressState::initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handle) {
    (void) si;
    (void) handle;
		logWithStateName(LogLevel::INFO, "Initiate upload of file again");
		//TODO: Implement
	}

	bool FileObjectUploadInProgressState::transfer(SessionInfoIf::SPtr si) {
		Buffer buffer;
		buffer << _transferNumber;

		auto response = _messageRouter->sendRequest(si,
				static_cast<cip::CipUsint>(FileObjectServiceCodes::UPLOAD_TRANSFER),
				cip::EPath(fileObject::FILE_OBJECT_CLASS_ID, _objectId),
				buffer.data());

		if (response.getGeneralStatusCode() == cip::GeneralStatusCodes::SUCCESS) {
			buffer = Buffer(response.getData());
			cip::CipUsint transferNumber;
			TransferPacketTypeCodes transferPacketType;


			buffer >> transferNumber >> reinterpret_cast<cip::CipUsint&>(transferPacketType);
			logWithStateName(LogLevel::DEBUG,
					"Receive transfer packet type=" + std::to_string(
							static_cast<cip::CipUsint>(transferPacketType)));

			if (transferNumber != _transferNumber) {
				logWithStateName(LogLevel::ERROR, "Wrong transfer number");
				_handler(cip::GeneralStatusCodes::INVALID_PARAMETER, std::vector<uint8_t>());
				return false;
			}

			switch (transferPacketType) {
				case TransferPacketTypeCodes::FIRST: {
					std::vector<uint8_t> fileData(_transferSize);
					buffer >> fileData;

					_fileContent.insert(_fileContent.end(), fileData.begin(), fileData.end());
					break;
				}
				case TransferPacketTypeCodes::MIDDLE: {
					std::vector<uint8_t> fileData(_transferSize);
					buffer >> fileData;

					_fileContent.insert(_fileContent.end(), fileData.begin(), fileData.end());
					break;
				}
				case TransferPacketTypeCodes::LAST:
				case TransferPacketTypeCodes::FIRST_AND_LAST: {
					std::vector<uint8_t> fileData(response.getData().size() - 4);	//TODO: Check CRC
					buffer >> fileData;

					_fileContent.insert(_fileContent.end(), fileData.begin(), fileData.end());
					if (_fileContent.size() == _fileSize) {
						_handler(cip::GeneralStatusCodes::SUCCESS, _fileContent);
					} else {
						logWithStateName(LogLevel::ERROR, "Wrong size of the uploaded file");
						_handler(cip::GeneralStatusCodes::INVALID_PARAMETER, std::vector<uint8_t>());
					}
					return false;
				}
				default:
					throw std::runtime_error("Received unexpected transfer type");
			};

			_transferNumber++;
		} else {
			logGeneralAndAdditionalStatus(response);
			_handler(response.getGeneralStatusCode(), std::vector<uint8_t>());
		}

		return true;
	}
}
}
