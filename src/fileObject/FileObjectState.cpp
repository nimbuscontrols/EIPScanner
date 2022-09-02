//
// Created by Aleksey Timin on 11/23/19.
//

#include <map>

#include "FileObjectState.h"
#include "utils/Buffer.h"
#include "FileObjectNonExistentState.h"
#include "FileObjectEmptyState.h"
#include "FileObjectLoadedState.h"
#include "FileObjectUploadInProgressState.h"

namespace eipScanner {
namespace fileObject {
	using utils::Logger;
	using utils::LogLevel;
	using utils::Buffer;

	FileObjectState::FileObjectState(FileObjectStateCodes state, FileObject &owner,
				cip::CipUint objectId, MessageRouter::SPtr messageRouter)
		: _messageRouter{messageRouter}
		, _objectId{objectId}
		, _stateCode{state}
		, _owner(owner) {

		logWithStateName(utils::LogLevel::DEBUG, "Start");
	}

	FileObjectState::~FileObjectState() {
		logWithStateName(utils::LogLevel::TRACE, "Stop");
	}

	FileObjectStateCodes FileObjectState::getStateCode() const {
		return _stateCode;
	}

	void FileObjectState::logWithStateName(LogLevel logLevel, const std::string &message) const {
		Logger(logLevel) << "FileObject=" << std::hex << _objectId << " <" << getStateName() << "> " << message;
	}

	std::string FileObjectState::getStateName() const {
		static const std::map<FileObjectStateCodes, std::string> NAMES_MAP = {
				std::make_pair(FileObjectStateCodes::NONEXISTENT, "NONEXISTENT"),
				std::make_pair(FileObjectStateCodes::FILE_LOADED, "FILE_LOADED"),
				std::make_pair(FileObjectStateCodes::FILE_EMPTY, "FILE_EMPTY"),
				std::make_pair(FileObjectStateCodes::TRANSFER_DOWNLOAD_IN_PROGRESS,
							   "TRANSFER_DOWNLOAD_IN_PROGRESS"),
				std::make_pair(FileObjectStateCodes::TRANSFER_DOWNLOAD_INITIATED, "TRANSFER_DOWNLOAD_INITIATED"),
				std::make_pair(FileObjectStateCodes::TRANSFER_UPLOAD_IN_PROGRESS, "TRANSFER_UPLOAD_IN_PROGRESS"),
				std::make_pair(FileObjectStateCodes::TRANSFER_UPLOAD_INITIATED, "TRANSFER_UPLOAD_INITIATED"),
				std::make_pair(FileObjectStateCodes::UNKNOWN, "UNKNOWN"),
		};

		return NAMES_MAP.at(_stateCode);
	}

	void FileObjectState::SyncState(SessionInfoIf::SPtr si) {
		auto response = _messageRouter->sendRequest(si, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
												   cip::EPath(FILE_OBJECT_CLASS_ID, _objectId,
															  static_cast<cip::CipUint>(FileObjectAttributesCodes::STATE)), {});

		if (response.getGeneralStatusCode() == cip::GeneralStatusCodes::SUCCESS) {
			Buffer buffer(response.getData());
			cip::CipUsint stateCode;
			buffer >> stateCode;

			switch (static_cast<FileObjectStateCodes>(stateCode)) {
				case FileObjectStateCodes::NONEXISTENT:
					setState<FileObjectNonExistentState>();
					break;
				case FileObjectStateCodes::FILE_EMPTY:
					setState<FileObjectEmptyState>();
					break;
				case FileObjectStateCodes::FILE_LOADED:
					setState<FileObjectLoadedState>();
					break;
				case FileObjectStateCodes::TRANSFER_UPLOAD_INITIATED:
				case FileObjectStateCodes::TRANSFER_UPLOAD_IN_PROGRESS:
					logWithStateName(LogLevel::WARNING, "File is uploading. "
										 "We need to re-initialize the uploading");
					setState<FileObjectLoadedState>();
					break;
				default:
					throw std::runtime_error("Read unknown or unsupported state");
			}

		} else {
			throw std::runtime_error("Failed load the state of File Object ID=" + std::to_string(_objectId));
		}
	}

	void FileObjectState::initiateUpload(SessionInfoIf::SPtr, EndUploadHandler) {
		logWithStateName(LogLevel::ERROR, "Not implemented call");
	}

	bool FileObjectState::transfer(SessionInfoIf::SPtr) {
		logWithStateName(LogLevel::ERROR, "Not implemented call");
		return false;
	}
}
}
