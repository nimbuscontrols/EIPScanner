//
// Created by Aleksey Timin on 11/21/19.
//

#ifndef EIPSCANNER_FILEOBJECT_H
#define EIPSCANNER_FILEOBJECT_H

#include <stdexcept>

#include "cip/Types.h"
#include "cip/GeneralStatusCodes.h"
#include "MessageRouter.h"
#include "BaseObject.h"

namespace eipScanner {
	namespace fileObject {
		class FileObjectState;
		using EndUploadHandler = std::function<void(cip::GeneralStatusCodes status, const std::vector<uint8_t>& fileContent)>;
	}

	/**
	 * the state codes of File Object
	 */
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
	* @class FileObject
	*
	* @brief Implements interface to File Object (0x37).
	*/
	class FileObject : public BaseObject {
		friend class fileObject::FileObjectState;
	public:
		using UPtr = std::unique_ptr<FileObject>;

		/**
		 * @brief Creates an instance and read the files object state
		 * @param instanceId the ID of the CIP instance
		 * @param si the EIP session for explicit messaging
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		FileObject(cip::CipUint instanceId, const SessionInfoIf::SPtr& si);

		/**
		 * @note used for testing
		 * @param instanceId
		 * @param si
		 * @param messageRouter
		 */
		FileObject(cip::CipUint instanceId, const SessionInfoIf::SPtr& si, const MessageRouter::SPtr& messageRouter);

		/**
		 * @brief Default destructor
		 */
		~FileObject();

		/**
		 * @brief Gets the current state of the file
		 * @return
		 */
		std::unique_ptr<fileObject::FileObjectState>&  getState();

		/**
		 * @brief Initiates uploading the file from the EIP adapter
		 * @param si the EIP session for explicit messaging
		 * @param handle a callback that called when the uploading finishes with an error or not
		 */
		void beginUpload(SessionInfoIf::SPtr si, fileObject::EndUploadHandler handle);

		/**
		 * @brief Handle upload transfers
		 * @return true if uploading is in progress
		 */
		bool handleTransfers(SessionInfoIf::SPtr si);


	private:
		std::unique_ptr<fileObject::FileObjectState> _state;
	};
}

#endif  // EIPSCANNER_FILEOBJECT_H
