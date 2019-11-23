//
// Created by Aleksey Timin on 11/23/19.
//

#include "FileObjectLoadedState.h"
#include "FileObjectUploadInProgressState.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace fileObject {
	using utils::Buffer;
	using utils::LogLevel;

	void FileObjectLoadedState::initiateUpload(SessionInfo::SPtr si, EndDownloadHandle handle) {
		logWithStateName(LogLevel::INFO, "Initiate upload");
		Buffer buffer;
		buffer << MAX_TRANSFER_SIZE;

		auto response = _messageRouter->sendRequest(si,
													static_cast<cip::CipUsint>(FileObjectServiceCodes::INITIATE_UPLOAD),
													cip::EPath(FILE_OBJECT_CLASS_ID, _objectId), buffer.data());

		if (response.getGeneralStatusCode() == cip::GeneralStatusCodes::SUCCESS) {
			buffer = Buffer(response.getData());
			cip::CipUdint fileSize = 0;
			cip::CipUsint transferSize = 0;

			buffer >> fileSize >> transferSize;
			setState<FileObjectUploadInProgressState>(fileSize, transferSize);

		} else {
			//TODO: error handling
		}
	}

	bool FileObjectLoadedState::transfer(SessionInfo::SPtr si) {
		return false;
	}
}
}