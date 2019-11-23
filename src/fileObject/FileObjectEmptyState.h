//
// Created by Aleksey Timin on 11/23/19.
//

#ifndef EIPSCANNER_FILEOBJECT_FILEOBJECTEMPTYSTATE_H
#define EIPSCANNER_FILEOBJECT_FILEOBJECTEMPTYSTATE_H

#include "FileObjectState.h"

namespace eipScanner {
namespace fileObject {

	class FileObjectEmptyState : public FileObjectState {
	public:
		using FileObjectState::FileObjectState;

		void initiateUpload(SessionInfo::SPtr si, EndDownloadHandle handle) override;

		bool transfer(SessionInfo::SPtr si) override;
	};
}
}

#endif  // EIPSCANNER_FILEOBJECT_FILEOBJECTEMPTYSTATE_H
