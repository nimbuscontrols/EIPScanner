//
// Created by Aleksey Timin on 11/23/19.
//

#ifndef EIPSCANNER_FILEOBJECT_FILEOBJECTLOADEDSTATE_H
#define EIPSCANNER_FILEOBJECT_FILEOBJECTLOADEDSTATE_H

#include "EIPScanner/fileObject/FileObjectState.h"

namespace eipScanner {
namespace fileObject {

	class FileObjectLoadedState : public FileObjectState {
	public:
		FileObjectLoadedState(FileObject &owner, cip::CipUint objectId, MessageRouter::SPtr messageRouter);

		void initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handler) override;
		bool transfer(SessionInfoIf::SPtr si) override;
	};

}
}

#endif  // EIPSCANNER_FILEOBJECTLOADEDSTATE_H
