//
// Created by Aleksey Timin on 11/23/19.
//

#include "FileObjectEmptyState.h"
namespace eipScanner {
namespace fileObject {
	using utils::LogLevel;

	FileObjectEmptyState::FileObjectEmptyState(FileObject &owner, cip::CipUint objectId,
											   MessageRouter::SPtr messageRouter)
			: FileObjectState(FileObjectStateCodes::FILE_EMPTY, owner, objectId, messageRouter) {
	}

	void FileObjectEmptyState::initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handle) {
		logWithStateName(LogLevel::WARNING, "File cannot be uploaded");
	}

	bool FileObjectEmptyState::transfer(SessionInfoIf::SPtr si) {
		logWithStateName(LogLevel::WARNING, "Nothing to transfer");
		return false;
	}

}
}