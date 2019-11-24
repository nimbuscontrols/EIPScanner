//
// Created by Aleksey Timin on 11/21/19.
//
#include <map>
#include "FileObject.h"
#include "fileObject/FileObjectState.h"
#include "utils/Buffer.h"
#include "utils/Logger.h"
#include "cip/Services.h"

namespace eipScanner {
	using utils::Buffer;
	using utils::Logger;
	using utils::LogLevel;



	FileObject::FileObject(cip::CipUint objectId, SessionInfo::SPtr si, MessageRouter::SPtr messageRouter)
		: _state(new fileObject::FileObjectState(FileObjectStateCodes::UNKNOWN, *this, objectId, messageRouter)) {
		_state->SyncState(si);
	}

	FileObject::~FileObject() = default;

	FileObjectStateCodes FileObject::getState() {
		return _state->getStateCode();
	}

	void FileObject::beginUpload(SessionInfo::SPtr si, fileObject::EndDownloadHandle handle) {
		_state->initiateUpload(si, handle);
	}

	bool FileObject::handleTransfers(SessionInfo::SPtr si) {
		return _state->transfer(si);
	}
}