//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_MESSAGEROUTER_H
#define EIPSCANNER_MESSAGEROUTER_H

#include <memory>
#include "cip/EPath.h"
#include "cip/Services.h"
#include "cip/MessageRouterResponse.h"
#include "eip/CommonPacketItem.h"
#include "SessionInfo.h"

namespace eipScanner {
	class MessageRouter {
	public:
		using SPtr = std::shared_ptr<MessageRouter>;

		MessageRouter();
		virtual ~MessageRouter();
		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
				const cip::EPath& path, const std::vector<uint8_t>& data,
				const std::vector<eip::CommonPacketItem>& additionalPacketItems) const;

		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
				const cip::EPath& path, const std::vector<uint8_t>& data) const;

		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
													   const cip::EPath& path) const;

	};
}


#endif  // EIPSCANNER_MESSAGEROUTER_H
