//
// Created by Aleksey Timin on 11/24/19.
//

#ifndef EIPSCANNER_TEST_FILEOBJECT_MOCKS_H
#define EIPSCANNER_TEST_FILEOBJECT_MOCKS_H

#include "../Mocks.h"
#include "EIPScanner/MessageRouter.h"
#include "EIPScanner/FileObject.h"
#include "EIPScanner/fileObject/FileObjectState.h"
#include "EIPScanner/utils/Buffer.h"

namespace eipScanner {
namespace fileObject {
	using utils::Buffer;
	using eipScanner::fileObject::FileObjectAttributesCodes;
	using eipScanner::fileObject::FILE_OBJECT_CLASS_ID;


	static void mockGetFileObjectState(TMockMessageRouter::SPtr messageRouter,
			SessionInfoIf::SPtr si, cip::CipUint objectId, FileObjectStateCodes stateCode)  {
		cip::MessageRouterResponse response;
		Buffer buffer;
		buffer << static_cast<cip::CipUsint>(stateCode);

		response.setData(buffer.data());
		response.setGeneralStatusCode(cip::SUCCESS);

		EXPECT_CALL(*messageRouter, sendRequest(si, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
												 cip::EPath(FILE_OBJECT_CLASS_ID, objectId,
															static_cast<cip::CipUint>(FileObjectAttributesCodes::STATE)),
												 std::vector<uint8_t>())
		).WillOnce(::testing::Return(response));
	}

}
}
#endif //EIPSCANNER_TEST_FILEOBJECT_MOCKS_H
