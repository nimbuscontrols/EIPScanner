//
// Created by James Roth on 12/19/19.
//

#ifndef EIPSCANNER_DPIFAULTPARAMETER_HPP
#define EIPSCANNER_DPIFAULTPARAMETER_HPP

#include "EIPScanner/cip/CipString.h"
#include "EIPScanner/BaseObject.h"
#include "EIPScanner/SessionInfoIf.h"
#include "EIPScanner/MessageRouter.h"
#include "EIPScanner/ParameterObject.h"
#include "EIPScanner/vendor/ra/powerFlex525/DPIFaultCode.h"

namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {

	class DPIFaultParameter {
	public:

		struct FaultDetails{
			int             faultNumber;
			cip::CipUint    faultCode;
			cip::CipLreal   busVoltage;
			cip::CipLreal   current;
			cip::CipLreal   frequency;
		};

		struct FullInformation {
			FaultDetails faultDetails;
			DPIFaultCode::FaultDescriptions faultDescription;
		};

		DPIFaultParameter(const SessionInfoIf::SPtr &si,
					   const MessageRouter::SPtr& messageRouter,
					   int faultNumber,
					   bool getFaultDetails);

		DPIFaultParameter();

		const FullInformation &getFullInformation() const;
		const FaultDetails &getFaultDetails() const; // returns struct fault details
		void setFaultDetails(FaultDetails faultInfo); // sets fault details struct in FullInformation struct
		void setFaultDescription(DPIFaultCode::FaultDescriptions faultDescriptions); // sets fault description (info mapped from fault code)

	private:

		FullInformation _fullInformation;
	};
}
}
}
}


#endif //EIPSCANNER_DPIFAULTPARAMETER_HPP
