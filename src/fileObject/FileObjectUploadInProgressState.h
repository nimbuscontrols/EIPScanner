//
// Created by Aleksey Timin on 11/23/19.
//

#ifndef EIPSCANNER_FILEOBJECT_FILEOBJECTUPLOADINPROGRESSSTATE_H
#define EIPSCANNER_FILEOBJECT_FILEOBJECTUPLOADINPROGRESSSTATE_H

#include <stdexcept>

#include "src/fileObject/FileObjectState.h"
#include "src/cip/Types.h"

namespace eipScanner {
namespace fileObject {
	class FileObjectUploadInProgressState : public FileObjectState {
	public:
		FileObjectUploadInProgressState(FileObject &owner, cip::CipUint objectId, MessageRouter::SPtr messageRouter,
										cip::CipUdint fileSize, cip::CipUsint transferSize, EndUploadHandler handler);

		void initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handle) override;
		bool transfer(SessionInfoIf::SPtr si) override;

	private:
		cip::CipUdint _fileSize;
		cip::CipUsint _transferSize;
		std::vector<uint8_t>_fileContent;
		cip::CipUsint _transferNumber;

		EndUploadHandler _handler;
	};

}
}
#endif  // EIPSCANNER_FILEOBJECT_FILEOBJECTUPLOADINPROGRESSSTATE_H
