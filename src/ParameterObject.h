//
// Created by Aleksey Timin on 12/4/19.
//

#ifndef EIPSCANNER_PARAMETEROBJECT_H
#define EIPSCANNER_PARAMETEROBJECT_H

#include <cmath>
#include "MessageRouter.h"
#include "utils/Buffer.h"
#include "BaseObject.h"

namespace eipScanner {

 /**
 * @class ParameterObject
 *
 * @brief Implements interface to Parameter Object (0x0F).
 *
 * It reads all data from the CIP instance in the constructor
 */
class ParameterObject : public BaseObject {
public:
	static const cip::CipUint CLASS_ID = 0x0f;

	/**
	 * @brief Creates an instance and reads all its data via EIP
	 *
	 * @param instanceId the ID of the CIP instance
	 * @param fullAttributes if true, then read all the attributes
	 * @param si the Session Info for explicit messaging
	 */
	ParameterObject(cip::CipUint instanceId, bool fullAttributes, const SessionInfoIf::SPtr& si);

	/**
	 * Creates an empty instance without any EIP requests
	 * @param instanceId the ID of the CIP instance
	 * @param fullAttributes if true, then read all the attributes
	 * @param typeSize the size of the value in bytes
	 */
	ParameterObject(cip::CipUint instanceId, bool fullAttributes, size_t typeSize);

	/**
	 * @brief Creates an instance and reads all its data via EIP
	 * @note Used for testing
	 *
	 * @param instanceId
	 * @param fullAttributes
	 * @param si
	 */
	ParameterObject(cip::CipUint instanceId, bool fullAttributes, const SessionInfoIf::SPtr& si, const MessageRouter::SPtr&);


	~ParameterObject();

	/**
	 * @brief Updates the parameter value from the instance
	 * @param si the Session Info for explicit messaging
	 */
	void updateValue(const SessionInfoIf::SPtr& si);

	template <typename T>
	T getActualValue() const {
		return encodeValue<T>(_value);
	}

	template <typename T>
	cip::CipLreal getEngValue() const {
		auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_value));
		return actualToEngValue(actualValue);
	}


	template <typename T>
	T getMinValue() const {
		return encodeValue<T>(_minValue);
	}

	template <typename T>
	cip::CipLreal getEngMinValue() const {
		auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_minValue));
		return actualToEngValue(actualValue);
	}

	template <typename T>
	void setEngMinValue(cip::CipLreal value) {
		T actualValue = static_cast<T>(engToActualValue(value));
		_minValue = decodeValue<T>(actualValue);
	}


	template <typename T>
	T getMaxValue() const {
		return encodeValue<T>(_maxValue);
	}

	template <typename T>
	cip::CipLreal getEngMaxValue() const {
		auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_maxValue));
		return actualToEngValue(actualValue);
	}

	template <typename T>
	void setEngMaxValue(cip::CipLreal value) {
		T actualValue = static_cast<T>(engToActualValue(value));
		_maxValue = decodeValue<T>(actualValue);
	}

	template <typename T>
	T getDefaultValue() const {
		return encodeValue<T>(_defaultValue);
	}

	template <typename T>
	cip::CipLreal getEngDefaultValue() const {
		auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_defaultValue));
		return actualToEngValue(actualValue);
	}

	template <typename T>
	void setEngDefaultValue(cip::CipLreal value) {
		T actualValue = static_cast<T>(engToActualValue(value));
		_defaultValue = decodeValue<T>(actualValue);
	}


	bool hasFullAttributes() const;
	bool isScalable() const;

	cip::CipDataTypes getType() const;
	const std::string &getName() const;
	const std::string &getUnits() const;
	const std::string &getHelp() const;
	cip::CipUint getScalingMultiplier() const;
	cip::CipUint getScalingDivisor() const;
	cip::CipUint getScalingBase() const;
	cip::CipInt getScalingOffset() const;
	cip::CipUsint getPrecision() const;

	void setScalable(bool isScalable);
	void setType(cip::CipDataTypes type);
	void setName(const std::string &name);
	void setUnits(const std::string &units);
	void setHelp(const std::string &help);
	void setScalingMultiplier(cip::CipUint scalingMultiplier);
	void setScalingDivisor(cip::CipUint scalingDivisor);
	void setScalingBase(cip::CipUint scalingBase);
	void setScalingOffset(cip::CipInt scalingOffset);
	void setPrecision(cip::CipUsint precision);

private:

	template <typename T>
	T encodeValue(const std::vector<uint8_t>& data) const {
		utils::Buffer buffer(data);
		T v;
		buffer >> v;
		return v;
	}

	template <typename T>
	std::vector<uint8_t> decodeValue(T v) const{
		utils::Buffer buffer(sizeof(T));
		buffer << v;
		return buffer.data();
	}

	cip::CipLreal actualToEngValue(cip::CipLreal actualValue) const;
	cip::CipLreal engToActualValue(cip::CipLreal engValue) const;

	bool _hasFullAttributes;
	bool _isScalable{};

	std::vector<uint8_t> _value;
	cip::CipDataTypes _type;
	std::string _name;
	std::string _units;
	std::string _help;
	std::vector<uint8_t> _minValue;
	std::vector<uint8_t> _maxValue;
	std::vector<uint8_t> _defaultValue;
	cip::CipUint _scalingMultiplier{};
	cip::CipUint _scalingDivisor{};
	cip::CipUint _scalingBase{};
	cip::CipInt _scalingOffset{};
	cip::CipUsint _precision{};

	MessageRouter::SPtr _messageRouter;
};

}
#endif  // EIPSCANNER_PARAMETEROBJECT_H
