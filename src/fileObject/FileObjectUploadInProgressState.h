//
// Created by Aleksey Timin on 11/23/19.
//

#ifndef EIPSCANNER_FILEOBJECT_FILEOBJECTUPLOADINPROGRESSSTATE_H
#define EIPSCANNER_FILEOBJECT_FILEOBJECTUPLOADINPROGRESSSTATE_H

#include "FileObjectState.h"
#include "cip/Types.h"

namespace eipScanner {
namespace fileObject {
	class FileObjectUploadInProgressState : public FileObjectState {
	public:
		FileObjectUploadInProgressState(FileObject &owner, cip::CipUint objectId, MessageRouter::SPtr messageRouter,
										cip::CipUdint fileSize, cip::CipUsint transferSize);

		void initiateUpload(SessionInfo::SPtr si, EndDownloadHandle handle) override;
		bool transfer(SessionInfo::SPtr si) override;

	private:
		cip::CipUdint _fileSize;
		cip::CipUsint _transferSize;
		std::string _fileContent;
		cip::CipUsint _transferNumber;
	};

}
}
#endif  // EIPSCANNER_FILEOBJECT_FILEOBJECTUPLOADINPROGRESSSTATE_H
