//
// Created by Aleksey Timin on 11/23/19.
//

#include "FileObjectEmptyState.h"
namespace eipScanner {
namespace fileObject {
	using utils::LogLevel;

	void FileObjectEmptyState::initiateUpload(SessionInfo::SPtr si, EndDownloadHandle handle) {
		logWithStateName(LogLevel::WARNING, "File cannot be uploaded");
	}

	bool FileObjectEmptyState::transfer(SessionInfo::SPtr si) {
		logWithStateName(LogLevel::WARNING, "Nothing to transfer");
		return false;
	}

}
}