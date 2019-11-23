//
// Created by Aleksey Timin on 11/23/19.
//

#include "FileObjectNonExistentState.h"

namespace eipScanner {
namespace fileObject {
	using utils::LogLevel;

	void FileObjectNonExistentState::initiateUpload(SessionInfo::SPtr si, EndDownloadHandle handle) {
		logWithStateName(LogLevel::WARNING,
						 "File cannot be uploaded");
	}

	bool FileObjectNonExistentState::transfer(SessionInfo::SPtr si) {
		logWithStateName(LogLevel::WARNING,
						 "Nothing to transfer");
		return false;
	}

}
}