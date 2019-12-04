//
// Created by Aleksey Timin on 11/16/19.
//
#include <stdexcept>
#include "MessageRouterResponse.h"
#include "utils/Buffer.h"
#include "utils/Logger.h"

namespace eipScanner {
namespace cip {
	using utils::Logger;
	using utils::LogLevel;
	using utils::Buffer;

	MessageRouterResponse::MessageRouterResponse()
		: _serviceCode{ServiceCodes::GET_ATTRIBUTE_ALL}
		, _generalStatusCode{GeneralStatusCodes::SUCCESS}
		, _additionalStatus(0)
		, _data(0) {
	}

	MessageRouterResponse::~MessageRouterResponse() = default;

	void MessageRouterResponse::expand(const std::vector<uint8_t> &data) {
		if (data.size() < 4) {
			throw std::runtime_error("Message Router response must have at least 4 bytes");
		}

		Buffer buffer(data);
		CipUsint reserved, additionalStatusSize;
		buffer >> reinterpret_cast<CipUsint&>(_serviceCode)
			>> reserved
			>> reinterpret_cast<CipUsint&>(_generalStatusCode)
			>> additionalStatusSize;

		if (additionalStatusSize*2 > data.size() - 4) {
			throw std::runtime_error("Additional status has wrong size");
		}

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

	void MessageRouterResponse::setGeneralStatusCode(GeneralStatusCodes generalStatusCode) {
		_generalStatusCode = generalStatusCode;
	}

	void MessageRouterResponse::setData(const std::vector<uint8_t> &data) {
		_data = data;
	}

	void logGeneralAndAdditionalStatus(const MessageRouterResponse &response) {
		Logger logger(LogLevel::ERROR);
		logger << "Message Router error=0x"
			   << std::hex << response.getGeneralStatusCode()
			   << " additional statuses ";
		for (auto& additionalStatus : response.getAdditionalStatus()) {
			logger << "[0x"
				   << additionalStatus
				   << "]";
		}
	}
}
}