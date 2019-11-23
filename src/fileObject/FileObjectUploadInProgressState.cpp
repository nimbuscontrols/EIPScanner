//
// Created by Aleksey Timin on 11/23/19.
//

#include "FileObjectUploadInProgressState.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace fileObject {
	using utils::LogLevel;
	using utils::Buffer;
	FileObjectUploadInProgressState::FileObjectUploadInProgressState(FileObject &owner, cip::CipUint objectId,
																	 MessageRouter::SPtr messageRouter,
																	 cip::CipUdint fileSize,
																	 cip::CipUsint transferSize)
			: FileObjectState{owner, objectId, messageRouter}, _fileSize{fileSize}, _transferSize{transferSize},
			  _fileContent(), _transferNumber{0} {

	}

	void
	FileObjectUploadInProgressState::initiateUpload(SessionInfo::SPtr si, EndDownloadHandle handle) {
		logWithStateName(LogLevel::INFO, "Initiate upload of file again");
		//TODO: Implement
	}

	bool FileObjectUploadInProgressState::transfer(SessionInfo::SPtr si) {
		Buffer buffer;
		buffer << _transferNumber++;

		return false;
	}
}
}