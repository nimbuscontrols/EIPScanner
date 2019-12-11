//
// Created by Aleksey Timin on 11/21/19.
//

#ifndef EIPSCANNER_FILEOBJECT_H
#define EIPSCANNER_FILEOBJECT_H


#include "cip/Types.h"
#include "cip/GeneralStatusCodes.h"
#include "MessageRouter.h"
#include "BaseObject.h"

namespace eipScanner {
	namespace fileObject {
		class FileObjectState;
		using EndUploadHandler = std::function<void(cip::GeneralStatusCodes status, const std::vector<uint8_t>& fileContent)>;
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

	/**
	 * Implements File Object
	 * @example
	 *
	 * 	auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);
	 * 	auto messageRouter = std::make_shared<MessageRouter>();
	 *
	 * 	FileObject edsFile(0xC8, si, messageRouter);
	 * 	edsFile.beginUpload(si, [](cip::GeneralStatusCodes status, const std::vector<uint8_t>& fileContent){
	 * 		if (status == cip::GeneralStatusCodes::SUCCESS) {
	 * 			// Process file
	 * 		}
	 * 	});
	 *
	 * 	while (edsFile.handleTransfers(si)) {
	 * 		continue;
	 * 	}
	 */
	class FileObject : public BaseObject {
		friend class fileObject::FileObjectState;
	public:
		using UPtr = std::unique_ptr<FileObject>;
		
		FileObject(cip::CipUint instanceId, SessionInfoIf::SPtr si, MessageRouter::SPtr messageRouter);
		~FileObject();
		std::unique_ptr<fileObject::FileObjectState>&  getState();
		void beginUpload(SessionInfoIf::SPtr si, fileObject::EndUploadHandler handle);

		/**
		 * handle active download/upload transfers
		 * @return true if downloading/uploading is in progress
		 */
		bool handleTransfers(SessionInfoIf::SPtr si);


	private:
		std::unique_ptr<fileObject::FileObjectState> _state;
	};
}

#endif  // EIPSCANNER_FILEOBJECT_H
