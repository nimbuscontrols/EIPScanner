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

	enum ParameterObjectAttributeIds : cip::CipUsint {
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

	enum DescriptorAttributeBits : cip::CipUint {
		SUPPORTS_SCALING = 1 << 2,
	};

	ParameterObject::ParameterObject(cip::CipUint id, bool fullAttributes,
									 const SessionInfo::SPtr &si)
		: ParameterObject(id, fullAttributes, si, std::make_shared<MessageRouter>()) {
	}

	ParameterObject::ParameterObject(cip::CipUint id, bool fullAttributes,
			const SessionInfo::SPtr &si,
			const MessageRouter::SPtr& messageRouter)
		: _instanceId{id}
		, _name{""}
		, _hasFullAttributes{fullAttributes}
		, _isScalable{false}
		, _messageRouter{messageRouter} {


		Logger(LogLevel::DEBUG) << "Read data from parameter ID=" << id;

		auto response = _messageRouter->sendRequest(si,
				ServiceCodes::GET_ATTRIBUTE_SINGLE,
				EPath(CLASS_ID, _instanceId,
						ParameterObjectAttributeIds::DATA_SIZE),{});


		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			CipUsint dataSize;
			Buffer buffer(response.getData());
			buffer >> dataSize;

			_value.resize(dataSize);
			_minValue.resize(dataSize);
			_maxValue.resize(dataSize);
			_defaultValue.resize(dataSize);
		} else {
			logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read data size of the parameter");
		}

		response = messageRouter->sendRequest(si,
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

			Logger(LogLevel::DEBUG) << "Parameter object ID=" << _instanceId
									<< " has descriptor=0x" << std::hex << descriptor
									<< " scalable=" << _isScalable;

			if (_hasFullAttributes) {
				ignore.resize(1);
				CipShortString  name, units, help;

				buffer >> ignore >> name >> units >> help
					>> _minValue >> _maxValue >> _defaultValue;

				_name = name.toStdString();
				_units = units.toStdString();
				_help = help.toStdString();

				_isScalable = descriptor & DescriptorAttributeBits::SUPPORTS_SCALING;
				if (_isScalable) {
					ignore.resize(8);
					buffer >> _scalingMultiplier >> _scalingDivisor
						>> _scalingBase >> _scalingOffset
						>> ignore
						>> _precision;


				}
			}

			if (!buffer.isValid()) {
				std::runtime_error("Not enough data in the response");
			}

			Logger(utils::DEBUG) << "Read Parameter Object"
				<< " ID=" << id
				<< " ValueSize=" << _value.size()
				<< " ValueType=0x" << std::hex << static_cast<int>(_type)
				<< " Name=" << _name;
		} else {
			cip::logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read all attributes");
		}
	}

	void ParameterObject::updateValue(const SessionInfo::SPtr& si) {
		auto response = _messageRouter->sendRequest(si,
								ServiceCodes::GET_ATTRIBUTE_SINGLE,
								EPath(CLASS_ID, _instanceId,
								ParameterObjectAttributeIds::VALUE),{});


		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			Buffer buffer(response.getData());
			buffer >> _value;
		} else {
			cip::logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read value");
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

	bool ParameterObject::hasFullAttributes() const {
		return _hasFullAttributes;
	}

	bool ParameterObject::isScalable() const {
		return _isScalable;
	}

	const std::string &ParameterObject::getUnits() const {
		return _units;
	}

	const std::string &ParameterObject::getHelp() const {
		return _help;
	}

	CipUint ParameterObject::getScalingMultiplier() const {
		return _scalingMultiplier;
	}

	CipUint ParameterObject::getScalingDivisor() const {
		return _scalingDivisor;
	}

	CipUint ParameterObject::getScalingBase() const {
		return _scalingBase;
	}

	CipInt ParameterObject::getScalingOffset() const {
		return _scalingOffset;
	}

	CipUsint ParameterObject::getPrecision() const {
		return _precision;
	}

};