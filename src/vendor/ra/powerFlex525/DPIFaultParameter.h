//
// Created by James Roth on 12/19/19.
//

#ifndef EIPSCANNER_DPIFAULTPARAMETER_HPP
#define EIPSCANNER_DPIFAULTPARAMETER_HPP



#include "cip/CipString.h"
#include "BaseObject.h"
#include "SessionInfoIf.h"
#include "MessageRouter.h"
#include "ParameterObject.h"
#include "DPIFaultCode.h"

namespace eipScanner {
	namespace vendor {
		namespace ra {
			namespace powerFlex525 {

				class DPIFaultParameter : public BaseObject {
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


					/**
					* Creates an instance and reads all its data via EIP
					* @param instanceId
					* @param fullAttributes if true, then read all the attributes
					* @param si
					*/
					/*DPIFaultObject(cip::CipUint instanceId,
								   const SessionInfoIf::SPtr &si);*/

					/*DPIFaultObject(cip::CipUint instanceId,
								   const SessionInfoIf::SPtr &si, const MessageRouter::SPtr& messageRouter);*/

					// for fault info at time of fault (volts, current, frequency)
					DPIFaultParameter(const SessionInfoIf::SPtr &si,
								   const MessageRouter::SPtr& messageRouter,
								   int faultNumber,
								   bool getFaultDetails);

					const FullInformation &getFullInformation() const;
					const FaultDetails &getFaultDetails() const; // returns struct fault details
					void setFaultDetails(FaultDetails faultInfo); // sets fault details struct in FullInformation struct
					void setFaultDescription(DPIFaultCode::FaultDescriptions faultDescriptions); // sets fault description (info mapped from fault code)

				private:

					FullInformation _fullInformation;
					FaultDetails _faultDetails; // stores volts, current & frequency at time of fault
				};
			}
		}
	}
}


#endif //EIPSCANNER_DPIFAULTPARAMETER_HPP
