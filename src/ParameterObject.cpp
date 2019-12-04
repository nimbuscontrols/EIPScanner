//
// Created by Aleksey Timin on 12/4/19.
//

#include "ParameterObject.h"
#include "utils/Buffer.h"
#include "utils/Logger.h"

namespace eipScanner {
	using utils::Buffer;
	using utils::Logger;
	using utils::LogLevel;
	using namespace ::eipScanner::cip;

	enum class ParameterObjectAttributeIds : cip::CipUsint {
		VALUE = 1,
		LINK_PATH_SIZE = 2,
		DESCRIPTOR = 4,
		DATA_TYPE = 5,
		DATA_SIZE = 6,
		NAME_STRING = 7,
		UNIT_STRING = 8,
		HELP_STRING = 9,
		MIN_VALUE = 10,
		MAX_VALUE = 11,
		DEFAULT_VALUE = 12,
		SCALING_MULTIPLIER = 13,
		SCALING_DIVISOR = 14,
		SCALING_BASE = 15,
		SCALING_OFFSET = 16
	};

	ParameterObject::ParameterObject(cip::CipUint id, bool fullAttributes,
									 const SessionInfo::WPtr &si)
		: ParameterObject(id, fullAttributes, si, std::make_shared<MessageRouter>()) {
	}

	ParameterObject::ParameterObject(cip::CipUint id, bool fullAttributes,
			const SessionInfo::WPtr &si,
			const MessageRouter::SPtr& messageRouter)
		: _instanceId{id}
		, _name{""} {


		Logger(LogLevel::DEBUG) << "Read data from parameter ID=" << id;

		auto response = messageRouter->sendRequest(si.lock(),
				ServiceCodes::GET_ATTRIBUTE_SINGLE,
				EPath(CLASS_ID, _instanceId,
						static_cast<CipUint>(ParameterObjectAttributeIds::DATA_SIZE)),{});


		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			CipUsint dataSize;
			Buffer buffer(response.getData());
			buffer >> dataSize;

			_value.resize(dataSize);
		} else {
			logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read data size of the parameter");
		}

		response = messageRouter->sendRequest(si.lock(),
				cip::ServiceCodes::GET_ATTRIBUTE_ALL, cip::EPath(CLASS_ID, _instanceId), {});

		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			Buffer buffer(response.getData());
			buffer >> _value;

			CipUsint linkPathSize;
			buffer >> linkPathSize;

			std::vector<uint8_t> ignore(linkPathSize);
			buffer >> ignore;

			CipWord descriptor;
			buffer >> descriptor >> reinterpret_cast<CipUsint&>(_type);

			if (fullAttributes) {
				ignore.resize(1);
				CipShortString  name;
				buffer >> ignore >> name;

				_name = name.toStdString();
			}

			if (!buffer.isValid()) {
				std::runtime_error("Not enough data in the response");
			}

			Logger(utils::DEBUG) << "Read Parameter Object"
				<< " ID=" << id
				<< " ValueSize=" << _value.size()
				<< " ValueType=" << static_cast<int>(_type)
				<< " Name=" << _name;
		} else {
			cip::logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read all attributes");
		}
	}

	ParameterObject::~ParameterObject() = default;

	const std::string &ParameterObject::getName() const {
		return _name;
	}

	cip::CipDataTypes ParameterObject::getType() const {
		return _type;
	}

	cip::CipUint ParameterObject::getInstanceId() const {
		return _instanceId;
	}


};