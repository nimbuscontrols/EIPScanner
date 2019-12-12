//
// Created by Aleksey Timin on 12/11/19.
//

#ifndef EIPSCANNER_VENDOR_DIPFAULTOBJECT_H
#define EIPSCANNER_VENDOR_DIPFAULTOBJECT_H

#include "cip/CipString.h"
#include "BaseObject.h"
#include "SessionInfoIf.h"
#include "MessageRouter.h"

namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {

	class DPIFaultObject : public BaseObject {
	public:
		static const cip::CipUint CLASS_ID = 0x97;

		struct FullInformation {
			cip::CipUint faultCode;
			cip::CipUsint dsiPort;
			cip::CipUsint dsiDeviceObject;
			cip::CipString faultText;
			cip::CipLword timerValue;
			bool isValidData;
			bool isRealTime;
		};

		/**
		* Creates an instance and reads all its data via EIP
		* @param instanceId
		* @param fullAttributes if true, then read all the attributes
		* @param si
		*/
		DPIFaultObject(cip::CipUint instanceId,
					   const SessionInfoIf::SPtr &si);

		DPIFaultObject(cip::CipUint instanceId,
					   const SessionInfoIf::SPtr &si, const MessageRouter::SPtr& messageRouter);

		const FullInformation &getFullInformation() const;

	private:
		FullInformation _fullInformation;
	};
}
}
}
}
#endif  // EIPSCANNER_VENDOR_DIPFAULTOBJECT_H
