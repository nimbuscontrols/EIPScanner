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
	T getValue() {
		utils::Buffer buffer(_value);
		T v;
		buffer >> v;

		return v;
	}

	cip::CipUint getInstanceId() const;
	const std::string &getName() const;
	cip::CipDataTypes getType() const;

private:
	cip::CipUint _instanceId;
	std::string _name;
	cip::CipDataTypes _type;

	std::vector<uint8_t> _value; //TODO: We need C+17 with std::any;
};

}
#endif  // EIPSCANNER_PARAMETEROBJECT_H
