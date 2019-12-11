//
// Created by Aleksey Timin on 12/11/19.
//

#include "DPIFaultObject.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace vendor {
namespace ra {
	using namespace cip;
	using utils::Buffer;
	
	enum DPIFaultObjectAttributeIds : CipUsint {
		FULL_INFORMATION = 0
	};

	DPIFaultObject::DPIFaultObject(CipUint instanceId, const SessionInfoIf::SPtr &si)
			: DPIFaultObject(instanceId, si, std::make_shared<MessageRouter>()) {

	}

	const DPIFaultObject::FullInformation &DPIFaultObject::getFullInformation() const {
		return _fullInformation;
	}

	DPIFaultObject::DPIFaultObject(CipUint instanceId, const SessionInfoIf::SPtr &si,
								   const MessageRouter::SPtr& messageRouter)
		: BaseObject(CLASS_ID, instanceId)
		, _fullInformation{0} {

		auto response = messageRouter->sendRequest(si, ServiceCodes::GET_ATTRIBUTE_SINGLE,
			EPath(CLASS_ID, instanceId, DPIFaultObjectAttributeIds::FULL_INFORMATION));

		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			Buffer buffer(response.getData());
			std::vector<uint8_t> faultText(16);
			buffer >> _fullInformation.faultCode
				>> _fullInformation.dsiPort
				>> _fullInformation.dsiDeviceObject
				>> faultText
				>> _fullInformation.timerValue;

			_fullInformation.faultText = CipString(faultText);

			std::vector<cip::CipBool> flags(16);
			buffer >> flags;
			_fullInformation.isValidData = flags[0];
			_fullInformation.isRealTime = flags[1];
		} else {
			logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read FULL_INFORMATION attribute");
		}
	}
}
}
}