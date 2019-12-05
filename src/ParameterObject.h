//
// Created by Aleksey Timin on 12/4/19.
//

#ifndef EIPSCANNER_PARAMETEROBJECT_H
#define EIPSCANNER_PARAMETEROBJECT_H

#include "MessageRouter.h"
#include "utils/Buffer.h"

namespace eipScanner {

class ParameterObject {
public:
	ParameterObject(cip::CipUint instanceId, bool fullAttributes, const SessionInfo::WPtr& si);
	ParameterObject(cip::CipUint instanceId, bool fullAttributes, const SessionInfo::WPtr& si, const MessageRouter::SPtr&);
	~ParameterObject();

	static const cip::CipUint CLASS_ID = 0x0f;

	template <typename T>
	T getActualValue() const {
		return encodeValue<T>(_value);
	}

	template <typename T>
	cip::CipLreal getEngValue() const {
		if (_isScalable) {
			auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_value));
			return ((actualValue + _scalingOffset) * _scalingMultiplier * _scalingBase) / _scalingDivisor;
		} else {
			return encodeValue<T>(_value);
		}
	}

	template <typename T>
	T getMinValue() const {
		return encodeValue<T>(_minValue);
	}

	template <typename T>
	T getMaxValue() const {
		return encodeValue<T>(_maxValue);
	}

	template <typename T>
	T getDefaultValue() const {
		return encodeValue<T>(_defaultValue);
	}

	bool hasFullAttributes() const;
	bool isScalable() const;

	const std::string &getUnits() const;
	const std::string &getHelp() const;
	cip::CipUint getScalingMultiplier() const;
	cip::CipUint getScalingDivisor() const;
	cip::CipUint getScalingBase() const;
	cip::CipUint getScalingOffset() const;
	cip::CipUint getInstanceId() const;
	cip::CipDataTypes getType() const;

	const std::string &getName() const;

private:

	template <typename T>
	T encodeValue(const std::vector<uint8_t>& data) const {
		utils::Buffer buffer(data);
		T v;
		buffer >> v;
		return v;
	}

	bool _hasFullAttributes;
	bool _isScalable;
	cip::CipUint _instanceId;

	std::vector<uint8_t> _value;
	cip::CipDataTypes _type;
	std::string _name;
	std::string _units;
	std::string _help;
	std::vector<uint8_t> _minValue;
	std::vector<uint8_t> _maxValue;
	std::vector<uint8_t> _defaultValue;
	cip::CipUint _scalingMultiplier;
	cip::CipUint _scalingDivisor;
	cip::CipUint _scalingBase;
	cip::CipUint _scalingOffset;
};

}
#endif  // EIPSCANNER_PARAMETEROBJECT_H
