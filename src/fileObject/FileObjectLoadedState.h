//
// Created by Aleksey Timin on 11/23/19.
//

#ifndef EIPSCANNER_FILEOBJECT_FILEOBJECTLOADEDSTATE_H
#define EIPSCANNER_FILEOBJECT_FILEOBJECTLOADEDSTATE_H

#include "FileObjectState.h"

namespace eipScanner {
namespace fileObject {

	class FileObjectLoadedState : public FileObjectState {
	public:
		FileObjectLoadedState(FileObject &owner, cip::CipUint objectId, MessageRouter::SPtr messageRouter);

		void initiateUpload(SessionInfo::SPtr si, EndDownloadHandle handle) override;
		bool transfer(SessionInfo::SPtr si) override;
	};

}
}

#endif  // EIPSCANNER_FILEOBJECTLOADEDSTATE_H
