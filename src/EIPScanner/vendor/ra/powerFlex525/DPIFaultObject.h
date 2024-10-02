//
// Created by Aleksey Timin on 12/11/19.
//

#ifndef EIPSCANNER_VENDOR_DIPFAULTOBJECT_H
#define EIPSCANNER_VENDOR_DIPFAULTOBJECT_H

#include <stdexcept>

#include "EIPScanner/cip/CipString.h"
#include "EIPScanner/BaseObject.h"
#include "EIPScanner/SessionInfoIf.h"
#include "EIPScanner/MessageRouter.h"

namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {

	/**
	 * @class DPIFaultObject
	 *
	 * @brief Implements interface to DPI Fault Object (0x97) of PowerFlex 525
	 */
	class DPIFaultObject : public BaseObject {
	public:
		static const cip::CipUint CLASS_ID = 0x97;

		/**
		 * Informaion about the fault
		 */
		struct FullInformation {
			cip::CipUint faultCode;			//!< the code of the fault (0 is no fault)
			cip::CipUsint dsiPort;			//!< DSI port
			cip::CipUsint dsiDeviceObject;  //!< DSI Device Object
			cip::CipString faultText;		//!< the text of the fault
			cip::CipLword timerValue;		//!< timer value
			bool isValidData;				//!< true if the timer value valid
			bool isRealTime;				//!< true if the time is real else it is elapsed
		};

		/**
		 * @brief Creates an instance and reads all its data via EIP
		 * @param instanceId
		 * @param fullAttributes if true, then read all the attributes
		 * @param si the EIP session for explicit messaging
		 */
		DPIFaultObject(cip::CipUint instanceId,
					   const SessionInfoIf::SPtr &si);

		/**
		 * @note used for testing
		 * @param instanceId
		 * @param si
		 * @param messageRouter
		 */
		DPIFaultObject(cip::CipUint instanceId,
					   const SessionInfoIf::SPtr &si, const MessageRouter::SPtr& messageRouter);

		/**
		 * @brief Gets the full information [AttrID=1] of the fault
		 * @return
		 */
		const FullInformation &getFullInformation() const;

	private:
		FullInformation _fullInformation;
	};
}
}
}
}
#endif  // EIPSCANNER_VENDOR_DIPFAULTOBJECT_H
