//
// Created by Aleksey Timin on 12/4/19.
//

#ifndef EIPSCANNER_PARAMETEROBJECT_H
#define EIPSCANNER_PARAMETEROBJECT_H

#include <cmath>
#include <stdexcept>
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
	 * @param instanceId the ID of the CIP instance
	 * @param fullAttributes if true, then read all the attributes including scaling attributes and text descriptions
	 * @param si the EIP session for explicit messaging
	 * @throw std::runtime_error
	 * @throw std::system_error
	 */
	ParameterObject(cip::CipUint instanceId, bool fullAttributes, const SessionInfoIf::SPtr& si);

	/**
	 * @brief Creates an empty instance without any EIP requests
	 * @param instanceId the ID of the CIP instance
 	 * @param fullAttributes if true, then read all the attributes including scaling attributes and text descriptions
	 * @param typeSize the size of the value in bytes
	 * @throw std::runtime_error
	 * @throw std::system_error
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

	/**
	 * @brief Default destructor
	 */
	~ParameterObject();

	/**
	 * @brief Updates the parameter value from the instance
	 * @param si the Session Info for explicit messaging
	 * @throw std::runtime_error
	 * @throw std::system_error
	 */
	void updateValue(const SessionInfoIf::SPtr& si);

	//// Flags from Descriptor Atribute [AttrID=4]
	/**
	 * @return true if the parameter supports scaling
	 */
	bool isScalable() const;

	/**
	 *
	 * @param isScalable true if the parameter supports scaling
	 */
	void setScalable(bool isScalable);

	/**
	 * @return true if the parameter value is read only
	 */
	bool isReadOnly() const;

	/**
	 *
	 * @param isReadOnly true if the parameter value is read only
	 */
	void setReadOnly(bool isReadOnly);

	/**
	 * @brief Gets an actual value [AttrID=1] of the parameter.
	 * @note This is just a getter. To read value from EIP device, use ParameterObject::updateValue
	 * @tparam T the type of the parameter
	 * @return the value of type T
	 */
	template <typename T>
	T getActualValue() const {
		return encodeValue<T>(_value);
	}

	/**
	 * @brief Gets a value of the parameter in EU
	 * For scaling the method uses scalling attributes (Multiplier [AttrID=13], Divisor [AttrID=14], Base [AttrID=15],
	 * Offset[ ID=16] and Precision [AttrID=21]
	 *
	 * The formula is: Value in EU = ((Actual Value + Offset)*Multiplier*Base)/(Divisor*10^Precision)
	 *
	 * @note it scales the actual value if ParameterObject::isScalable is true, else it returns the original actual value
	 * @tparam T the type of the parameter
	 * @return the value in EU
	 */
	template <typename T>
	cip::CipLreal getEngValue() const {
		auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_value));
		return actualToEngValue(actualValue);
	}

	/**
	 * @brief Gets a minimal value [AttrID=10] of the parameter.
	 * @note The behavior is the same as ParameterObject::getActualValue
	 * @tparam T the type of the parameter
	 * @return the value of type T
	 */
	template <typename T>
	T getMinValue() const {
		return encodeValue<T>(_minValue);
	}

	/**
	 * @brief Gets a minimal value of the parameter in EU
	 * @note The behavior is the same as ParameterObject::getEngValue
	 * @tparam T the type of the parameter
	 * @return the value in EU
	 */
	template <typename T>
	cip::CipLreal getEngMinValue() const {
		auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_minValue));
		return actualToEngValue(actualValue);
	}

	/**
	 * @brief Sets a minimal value of the parameter in EU
	 * @tparam T the type of the parameter
	 * @param value
	 */
	template <typename T>
	void setEngMinValue(cip::CipLreal value) {
		T actualValue = static_cast<T>(engToActualValue(value));
		_minValue = decodeValue<T>(actualValue);
	}

	/**
	 * @brief Gets a maximal value [AttrID=11] of the parameter.
	 * @note The behavior is the same as ParameterObject::getActualValue
	 * @tparam T the type of the parameter
	 * @return the value of type T
	 */
	template <typename T>
	T getMaxValue() const {
		return encodeValue<T>(_maxValue);
	}

	/**
	 * @brief Gets a maximal value of the parameter in EU
	 * @note The behavior is the same as ParameterObject::getEngValue
	 * @tparam T the type of the parameter
	 * @return the value in EU
	 */
	template <typename T>
	cip::CipLreal getEngMaxValue() const {
		auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_maxValue));
		return actualToEngValue(actualValue);
	}

	/**
	 * @brief Sets a maximal value of the parameter in EU
	 * @tparam T the type of the parameter
	 * @param value
	 */
	template <typename T>
	void setEngMaxValue(cip::CipLreal value) {
		T actualValue = static_cast<T>(engToActualValue(value));
		_maxValue = decodeValue<T>(actualValue);
	}

	/**
	 * @brief Gets an default value [AttrID=12] of the parameter.
	 * @note The behavior is the same as ParameterObject::getActualValue
	 * @tparam T the type of the parameter
	 * @return the value of type T
	 */
	template <typename T>
	T getDefaultValue() const {
		return encodeValue<T>(_defaultValue);
	}

	/**
	 * @brief Gets a default value of the parameter in EU
	 * @note The behavior is the same as ParameterObject::getEngValue
	 * @tparam T the type of the parameter
	 * @return the value in EU
	 */
	template <typename T>
	cip::CipLreal getEngDefaultValue() const {
		auto actualValue = static_cast<cip::CipLreal>(encodeValue<T>(_defaultValue));
		return actualToEngValue(actualValue);
	}

	/**
	 * @brief Sets a default value of the parameter in EU
	 * @tparam T the type of the parameter
	 * @param value
	 */
	template <typename T>
	void setEngDefaultValue(cip::CipLreal value) {
		T actualValue = static_cast<T>(engToActualValue(value));
		_defaultValue = decodeValue<T>(actualValue);
	}

	/**
	 * @return true if the parameter supports full data including scaling attributes and text descriptions
	 */
	bool hasFullAttributes() const;

	/**
	 * @brief Gets the type code [AttrID=5] of the parameter
	 * @return
	 */
	cip::CipDataTypes getType() const;

	/**
	 * @brief Sets the type code [AttrID=5] of the parameter
	 * @param type
	 */
	void setType(cip::CipDataTypes type);

	/**
	 * @brief Gets the name [AttrID=7] of the parameter
	 * @return
	 */
	const std::string &getName() const;

	/**
	 * @brief Gets the units [AttrID=8] of the parameter
	 * @return
	 */
	const std::string &getUnits() const;

	/**
	 * @brief Gets the help string [AttrID=9] of the parameter
	 * @return
	 */
	const std::string &getHelp() const;

	/**
	 * @brief Gets the number of parameter (instance ID)
	 * @return
	 */
	 const cip::CipUint &getParameter() const;

	/**
	 * @brief Sets the name [AttrID=7] of the parameter
	 * @param name
	 */
	void setName(const std::string &name);

	/**
	 * @brief Sets the units [AttrID=8] of the parameter
	 * @param units
	 */
	void setUnits(const std::string &units);

	/**
	 * @brief Sets the help string [AttrID=9] of the parameter
	 * @param help
	 */
	void setHelp(const std::string &help);

	/**
	 * @brief Gets the multiplier [AttrID=13] of the parameter
	 * @return
	 */
	cip::CipUint getScalingMultiplier() const;

	/**
	 * @brief Gets the divisor [AttrID=14] of the parameter
	 * @return
	 */
	cip::CipUint getScalingDivisor() const;

	/**
	 * @brief Gets the base [AttrID=15] of the parameter
	 * @return
	 */
	cip::CipUint getScalingBase() const;

	/**
	 * @brief Gets the offset [AttrID=16] of the parameter
	 * @return
	 */
	cip::CipInt getScalingOffset() const;

	/**
	 * @brief Gets the precision [AttrID=21] of the parameter
	 * @return
	 */
	cip::CipUsint getPrecision() const;

	/**
	 * @brief Sets the multiplier [AttrID=13] of the parameter
	 * @param scalingMultiplier
	 */
	void setScalingMultiplier(cip::CipUint scalingMultiplier);

	/**
	 * @brief Sets the divisor [AttrID=14] of the parameter
	 * @param scalingDivisor
	 */
	void setScalingDivisor(cip::CipUint scalingDivisor);

	/**
	 * @brief Sets the base [AttrID=15] of the parameter
	 * @param scalingBase
	 */
	void setScalingBase(cip::CipUint scalingBase);

	/**
	 * @brief Sets the offset [AttrID=16] of the parameter
	 * @param scalingOffset
	 */
	void setScalingOffset(cip::CipInt scalingOffset);

	/**
	 * @brief Sets the precision [AttrID=21] of the parameter
	 * @param precision
	 */
	void setPrecision(cip::CipUsint precision);

    cip::CipLreal actualToEngValue(cip::CipLreal actualValue) const;
    cip::CipLreal engToActualValue(cip::CipLreal engValue) const;

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

	bool _hasFullAttributes;
	bool _isScalable;
	bool _isReadOnly;

    cip::CipUint _parameter;
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
	cip::CipInt _scalingOffset;
	cip::CipUsint _precision;

	MessageRouter::SPtr _messageRouter;
};

}
#endif  // EIPSCANNER_PARAMETEROBJECT_H
