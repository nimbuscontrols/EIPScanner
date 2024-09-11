//
// Created by Aleksey Timin on 12/4/19.
//

#include "EIPScanner/ParameterObject.h"
#include "EIPScanner/utils/Buffer.h"
#include "EIPScanner/utils/Logger.h"

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
		READ_ONLY = 1 << 4,
	};

	ParameterObject::ParameterObject(cip::CipUint id, bool fullAttributes,
									 const SessionInfoIf::SPtr &si)
		: ParameterObject(id, fullAttributes, si, std::make_shared<MessageRouter>()) {
	}

    ParameterObject::ParameterObject(cip::CipUint instanceId, bool fullAttributes, size_t typeSize)
            :  BaseObject(CLASS_ID, instanceId)
            , _hasFullAttributes(fullAttributes)
            , _isScalable(false)
            , _isReadOnly(false)
            , _parameter(0)
            , _value(typeSize)
            , _type(CipDataTypes::ANY)
            , _name{""}
            , _minValue(typeSize)
            , _maxValue(typeSize)
            , _defaultValue(typeSize)
            , _scalingMultiplier(1)
            , _scalingDivisor(1)
            , _scalingBase(1)
            , _scalingOffset(0)
            , _precision(0) {
    }

    ParameterObject::ParameterObject(cip::CipUint instanceId, bool fullAttributes,
                                     const SessionInfoIf::SPtr &si,
                                     const MessageRouter::SPtr& messageRouter)
            :  BaseObject(CLASS_ID, instanceId)
            , _hasFullAttributes(fullAttributes)
            , _isScalable(false)
            , _isReadOnly(false)
            , _parameter(0)
            , _value(0)
            , _type(CipDataTypes::ANY)
            , _name{""}
            , _minValue(0)
            , _maxValue(0)
            , _defaultValue(0)
            , _scalingMultiplier(1)
            , _scalingDivisor(1)
            , _scalingBase(1)
            , _scalingOffset(0)
            , _precision(0)
            , _messageRouter{messageRouter} {


		Logger(LogLevel::DEBUG) << "Read data from parameter ID=" << instanceId;

		auto response = _messageRouter->sendRequest(si,
				ServiceCodes::GET_ATTRIBUTE_SINGLE,
				EPath(CLASS_ID, instanceId,
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
				cip::ServiceCodes::GET_ATTRIBUTE_ALL, cip::EPath(CLASS_ID, instanceId), {});

		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			Buffer buffer(response.getData());
			buffer >> _value;

			CipUsint linkPathSize;
			buffer >> linkPathSize;

			std::vector<uint8_t> ignore(linkPathSize);
			buffer >> ignore;

			CipWord descriptor;
			buffer >> descriptor >> reinterpret_cast<CipUsint&>(_type);

            _parameter = instanceId;
            _isScalable = descriptor & DescriptorAttributeBits::SUPPORTS_SCALING;
			_isReadOnly = descriptor & DescriptorAttributeBits::READ_ONLY;
			Logger(LogLevel::DEBUG) << "Parameter object ID=" << instanceId
									<< " has descriptor=0x" << std::hex << descriptor
									<< " scalable=" << _isScalable
									<< " readonly=" << _isReadOnly;

			if (_hasFullAttributes) {
				ignore.resize(1);
				CipShortString  name, units, help;

				buffer >> ignore >> name >> units >> help
					>> _minValue >> _maxValue >> _defaultValue;

				_name = name.toStdString();
				_units = units.toStdString();
				_help = help.toStdString();

				if (_isScalable) {
					ignore.resize(16); // Ignore scaling attributes we read it separately due to a bug
					buffer >> ignore
						>> _precision;

					std::vector<uint8_t> data;
					for (int attrId = ParameterObjectAttributeIds::SCALING_MULTIPLIER;
						 attrId <= ParameterObjectAttributeIds::SCALING_OFFSET;
						 ++attrId) {
						auto response = _messageRouter->sendRequest(si,
															   ServiceCodes::GET_ATTRIBUTE_SINGLE,
															   EPath(CLASS_ID, instanceId, attrId),
															   {});

						if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
							cip::logGeneralAndAdditionalStatus(response);
							throw std::runtime_error("Failed to read value of attribute=" + std::to_string(attrId));
						}

						data.insert(data.end(), response.getData().begin(), response.getData().end());
					}

					buffer = Buffer(data);
					buffer >> _scalingMultiplier >> _scalingDivisor
						>> _scalingBase >> _scalingOffset;
				}
			}

			if (!buffer.isValid()) {
				std::runtime_error("Not enough data in the response");
			}

			Logger(LogLevel::DEBUG) << "Read Parameter Object"
				<< " ID=" << instanceId
				<< " ValueSize=" << _value.size()
				<< " ValueType=0x" << std::hex << static_cast<int>(_type)
				<< " Name=" << _name;
		} else {
			cip::logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read all attributes");
		}
	}

	void ParameterObject::updateValue(const SessionInfoIf::SPtr& si) {
		auto response = _messageRouter->sendRequest(si,
								ServiceCodes::GET_ATTRIBUTE_SINGLE,
								EPath(CLASS_ID, getInstanceId(),
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

	bool ParameterObject::hasFullAttributes() const {
		return _hasFullAttributes;
	}

	bool ParameterObject::isScalable() const {
		return _isScalable;
	}

	bool ParameterObject::isReadOnly() const {
		return _isReadOnly;
	}

	const std::string &ParameterObject::getUnits() const {
		return _units;
	}

	const std::string &ParameterObject::getHelp() const {
		return _help;
	}

    const cip::CipUint &ParameterObject::getParameter() const {
	    return _parameter;
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

	void ParameterObject::setScalable(bool isScalable) {
		_isScalable = isScalable;
	}

	void ParameterObject::setReadOnly(bool isReadOnly) {
		_isReadOnly = isReadOnly;
	}

	void ParameterObject::setType(CipDataTypes type) {
		_type = type;
	}

	void ParameterObject::setName(const std::string &name) {
		_name = name;
	}

	void ParameterObject::setUnits(const std::string &units) {
		_units = units;
	}

	void ParameterObject::setHelp(const std::string &help) {
		_help = help;
	}

	void ParameterObject::setScalingMultiplier(CipUint scalingMultiplier) {
		_scalingMultiplier = scalingMultiplier;
	}

	void ParameterObject::setScalingDivisor(CipUint scalingDivisor) {
		_scalingDivisor = scalingDivisor;
	}

	void ParameterObject::setScalingBase(CipUint scalingBase) {
		_scalingBase = scalingBase;
	}

	void ParameterObject::setScalingOffset(CipInt scalingOffset) {
		_scalingOffset = scalingOffset;
	}

	void ParameterObject::setPrecision(CipUsint precision) {
		_precision = precision;
	}

	cip::CipLreal ParameterObject::actualToEngValue(cip::CipLreal actualValue) const {
		if (_isScalable) {
			return ((actualValue + _scalingOffset) * _scalingMultiplier * _scalingBase)
				   / (_scalingDivisor * std::pow(10, _precision));
		} else {
			return actualValue;
		}
	}

	cip::CipLreal ParameterObject::engToActualValue(cip::CipLreal engValue) const {
		if (_isScalable) {
			return (engValue * _scalingDivisor * std::pow(10, _precision))
				   / (_scalingMultiplier * _scalingBase) - _scalingOffset;
		} else {
			return engValue;
		}
	}
}
