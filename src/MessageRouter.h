//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_MESSAGEROUTER_H
#define EIPSCANNER_MESSAGEROUTER_H

#include <memory>
#include "cip/EPath.h"
#include "cip/Services.h"
#include "cip/MessageRouterResponse.h"
#include "SessionInfo.h"

namespace eipScanner {
	class MessageRouter {
	public:
		using SPtr = std::shared_ptr<MessageRouter>;

		explicit MessageRouter(SessionInfo::SPtr si);
		cip::MessageRouterResponse sendRequest(cip::CipUsint service,
				const cip::EPath& path, const std::vector<uint8_t>& data);

	private:
		SessionInfo::SPtr _si;
	};
}


#endif  // EIPSCANNER_MESSAGEROUTER_H
