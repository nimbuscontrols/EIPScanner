//
// Created by Aleksey Timin on 11/23/19.
//

#include "src/FileObjectLoadedState.h"
#include "src/FileObjectUploadInProgressState.h"
#include "src/utils/Buffer.h"

namespace eipScanner {
namespace fileObject {
	using utils::Buffer;
	using utils::LogLevel;

	FileObjectLoadedState::FileObjectLoadedState(FileObject &owner, cip::CipUint objectId,
											   MessageRouter::SPtr messageRouter)
			: FileObjectState(FileObjectStateCodes::FILE_LOADED, owner, objectId, messageRouter) {
	}

	void FileObjectLoadedState::initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handler) {
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
			setState<FileObjectUploadInProgressState>(fileSize, transferSize, handler);

		} else {
			logGeneralAndAdditionalStatus(response);
			handler(response.getGeneralStatusCode(), std::vector<uint8_t>(0));
		}
	}

	bool FileObjectLoadedState::transfer(SessionInfoIf::SPtr si) {
		return false;
	}
}
}