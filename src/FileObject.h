//
// Created by Aleksey Timin on 11/21/19.
//

#ifndef EIPSCANNER_FILEOBJECT_H
#define EIPSCANNER_FILEOBJECT_H


#include "cip/Types.h"
#include "cip/GeneralStatusCodes.h"
#include "MessageRouter.h"

namespace eipScanner {
	namespace fileObject {
		class FileObjectState;
		using EndDownloadHandle = std::function<void(cip::GeneralStatusCodes status, const std::string& fileContent)>;
	}

	enum class FileObjectStateCodes : cip::CipUsint {
		NONEXISTENT = 0,
		FILE_EMPTY = 1,
		FILE_LOADED = 2,
		TRANSFER_UPLOAD_INITIATED = 3,
		TRANSFER_DOWNLOAD_INITIATED = 4,
		TRANSFER_UPLOAD_IN_PROGRESS = 5,
		TRANSFER_DOWNLOAD_IN_PROGRESS = 6,

		UNKNOWN = 255,
	};

	class FileObject {
		friend class fileObject::FileObjectState;
	public:
		using UPtr = std::unique_ptr<FileObject>;
		
		FileObject(cip::CipUint objectId, SessionInfo::SPtr si, MessageRouter::SPtr messageRouter);
		~FileObject();
		FileObjectStateCodes getState();
		void beginUpload(SessionInfo::SPtr si, fileObject::EndDownloadHandle handle);

		/**
		 * handle active download/upload transfers
		 * @return true if downloading/uploading is in progress
		 */
		bool handleTransfers(SessionInfo::SPtr si);


	private:
		std::unique_ptr<fileObject::FileObjectState> _state;
	};
}

#endif  // EIPSCANNER_FILEOBJECT_H
