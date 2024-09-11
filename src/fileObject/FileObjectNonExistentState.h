//
// Created by Aleksey Timin on 11/23/19.
//

#ifndef EIPSCANNER_FILEOBJECT_FILEOBJECTNONEXISTENTSTATE_H
#define EIPSCANNER_FILEOBJECT_FILEOBJECTNONEXISTENTSTATE_H

#include "src/FileObjectState.h"

namespace eipScanner {
namespace fileObject {

	class FileObjectNonExistentState : public FileObjectState {
	public:
		FileObjectNonExistentState(FileObject &owner, cip::CipUint objectId, MessageRouter::SPtr messageRouter);

		void initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handle) override;
		bool transfer(SessionInfoIf::SPtr si) override;
	};

}
}
#endif  // EIPSCANNER_FILEOBJECTNONEXISTENTSTATE_H
