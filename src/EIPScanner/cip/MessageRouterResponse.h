//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_MESSAGEROUTERRESPONSE_H
#define EIPSCANNER_CIP_MESSAGEROUTERRESPONSE_H

#include <cstdint>
#include <vector>

#include "EIPScanner/cip/GeneralStatusCodes.h"

#include "EIPScanner/cip/Services.h"
#include "EIPScanner/eip/CommonPacketItem.h"

namespace eipScanner {
namespace cip {
	class MessageRouterResponse {
	public:
		MessageRouterResponse();
		~MessageRouterResponse();

		void expand(const std::vector<uint8_t>& data);

		GeneralStatusCodes getGeneralStatusCode() const;
		ServiceCodes getServiceCode() const;
		const std::vector<uint16_t> &getAdditionalStatus() const;
		const std::vector<uint8_t> &getData() const;
		const eip::CommonPacketItem::Vec &getAdditionalPacketItems() const;

		void setGeneralStatusCode(GeneralStatusCodes generalStatusCode);
		void setData(const std::vector<uint8_t> &data);

		void setAdditionalPacketItems(const std::vector<eip::CommonPacketItem> &_additionalPacketItems);

	private:
		ServiceCodes _serviceCode;
		GeneralStatusCodes _generalStatusCode;
		std::vector<uint16_t> _additionalStatus;
		std::vector<uint8_t> _data;
		eip::CommonPacketItem::Vec _additionalPacketItems;
	};

	void logGeneralAndAdditionalStatus(const MessageRouterResponse& response);
}
}

#endif  // EIPSCANNER_CIP_MESSAGEROUTERRESPONSE_H
