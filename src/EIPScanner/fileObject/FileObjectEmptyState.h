//
// Created by Aleksey Timin on 11/23/19.
//

#ifndef EIPSCANNER_FILEOBJECT_FILEOBJECTEMPTYSTATE_H
#define EIPSCANNER_FILEOBJECT_FILEOBJECTEMPTYSTATE_H

#include "EIPScanner/fileObject/FileObjectState.h"

namespace eipScanner {
namespace fileObject {

	class FileObjectEmptyState : public FileObjectState {
	public:
		FileObjectEmptyState(FileObject &owner, cip::CipUint objectId, MessageRouter::SPtr messageRouter);
		void initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handle) override;

		bool transfer(SessionInfoIf::SPtr si) override;
	};
}
}

#endif  // EIPSCANNER_FILEOBJECT_FILEOBJECTEMPTYSTATE_H
