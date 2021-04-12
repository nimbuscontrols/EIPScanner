//
// Created by Aleksey Timin on 11/23/19.
//

#include "FileObjectNonExistentState.h"

namespace eipScanner {
namespace fileObject {
	using utils::LogLevel;

	FileObjectNonExistentState::FileObjectNonExistentState(FileObject &owner, cip::CipUint objectId,
												 MessageRouter::SPtr messageRouter)
			: FileObjectState(FileObjectStateCodes::NONEXISTENT, owner, objectId, messageRouter) {
	}

	void FileObjectNonExistentState::initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handle) {
    (void) si;
    (void) handle;
		logWithStateName(LogLevel::WARNING,
						 "File cannot be uploaded");
	}

	bool FileObjectNonExistentState::transfer(SessionInfoIf::SPtr si) {
    (void) si;
		logWithStateName(LogLevel::WARNING,
						 "Nothing to transfer");
		return false;
	}

}
}
