//
// Created by Aleksey Timin on 12/9/19.
//

#ifndef EIPSCANNER_MOCKS_H
#define EIPSCANNER_MOCKS_H

#include <gmock/gmock.h>
#include "MessageRouter.h"

class TMockMessageRouter : public eipScanner::MessageRouter {
public:
	using SPtr = std::shared_ptr<TMockMessageRouter>;

	MOCK_CONST_METHOD4(sendRequest, eipScanner::cip::MessageRouterResponse(
			eipScanner::SessionInfo::SPtr si,
			eipScanner::cip::CipUsint service,
			const eipScanner::cip::EPath &path,
			const std::vector<uint8_t> &data));
};


#endif //EIPSCANNER_MOCKS_H
