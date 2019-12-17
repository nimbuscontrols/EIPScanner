//
// Created by Aleksey Timin on 12/11/19.
//

#ifndef EIPSCANNER_VENDOR_DIPFAULTOBJECT_H
#define EIPSCANNER_VENDOR_DIPFAULTOBJECT_H

#include "cip/CipString.h"
#include "BaseObject.h"
#include "SessionInfoIf.h"
#include "MessageRouter.h"
#include "ParameterObject.h"

namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {

	class DPIFaultObject : public BaseObject {
	public:
		static const cip::CipUint CLASS_ID = 0x97;

        struct FaultDetails{
            int             faultNumber;
            cip::CipLreal   busVoltage;
            cip::CipLreal   current;
            cip::CipLreal   frequency;
            //cip::CipLword status;
        };

		struct FullInformation {
			cip::CipUint faultCode;
			cip::CipUsint dsiPort;
			cip::CipUsint dsiDeviceObject;
			cip::CipString faultText;
			cip::CipLword timerValue;
			bool isValidData;
			bool isRealTime;

            FaultDetails faultDetails;
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

		// for fault info at time of fault (volts, current, frequency)
        DPIFaultObject(const SessionInfoIf::SPtr &si,
                                       const MessageRouter::SPtr& messageRouter,
                                       int faultNumber);

		const FullInformation &getFullInformation() const;
        const FaultDetails &getFaultDetails() const; // returns struct fault details
        void setFaultDetails(FaultDetails faultInfo); // sets fault details struct in FullInformation struct

	private:

		FullInformation _fullInformation;
        FaultDetails _faultDetails; // stores volts, current & frequency at time of fault
	};
}
}
}
}
#endif  // EIPSCANNER_VENDOR_DIPFAULTOBJECT_H
