//
// Created by Aleksey Timin on 11/16/19.
//

#include "MessageRouterResponse.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {

	using utils::Buffer;

	MessageRouterResponse::MessageRouterResponse()
		: _serviceCode{ServiceCodes::GET_ATTRIBUTE_ALL}
		, _generalStatusCode{GeneralStatusCodes::SUCCESS}
		, _additionalStatus(0)
		, _data(0) {

	}

	MessageRouterResponse::~MessageRouterResponse() = default;

	void MessageRouterResponse::expand(const std::vector<uint8_t> &data) {
		Buffer buffer(data);
		CipUsint reserved, additionalStatusSize;
		buffer >> reinterpret_cast<CipUsint&>(_serviceCode)
			>> reserved
			>> reinterpret_cast<CipUsint&>(_generalStatusCode)
			>> additionalStatusSize;

		_additionalStatus.resize(additionalStatusSize);
		buffer >> _additionalStatus;

		_data.resize(buffer.size() - buffer.pos());
		buffer >> _data;
	}

	ServiceCodes MessageRouterResponse::getServiceCode() const {
		return _serviceCode;
	}

	GeneralStatusCodes MessageRouterResponse::getGeneralStatusCode() const {
		return _generalStatusCode;
	}

	const std::vector<uint16_t> &MessageRouterResponse::getAdditionalStatus() const {
		return _additionalStatus;
	}

	const std::vector<uint8_t> &MessageRouterResponse::getData() const {
		return _data;
	}
}
}