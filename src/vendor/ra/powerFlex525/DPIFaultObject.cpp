//
// Created by Aleksey Timin on 12/11/19.
//

#include "DPIFaultObject.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {

	using namespace cip;
	using utils::Buffer;

	enum DPIFaultObjectAttributeIds : CipUsint {
		FULL_INFORMATION = 0
	};

	enum FaultTimeStampFlags : CipUint {
		VALID_DATA = 1,
		REAL_TIME = 1 << 1
	};

	enum FaultParams : CipUint {
        FAULT_1_CODE = 7,
        FAULT_2_CODE = 8,
        FAULT_3_CODE = 9,
        FAULT_4_CODE = 604,
        FAULT_5_CODE = 605,
        FAULT_6_CODE = 606,
        FAULT_7_CODE = 607,
        FAULT_8_CODE = 608,
        FAULT_9_CODE = 609,
        FAULT_10_CODE = 610,

        /*FAULT_1_TIME_HR = 611,
        FAULT_2_TIME_HR = 612,
        FAULT_3_TIME_HR = 613,
        FAULT_4_TIME_HR = 614,
        FAULT_5_TIME_HR = 615,
        FAULT_6_TIME_HR = 616,
        FAULT_7_TIME_HR = 617,
        FAULT_8_TIME_HR = 618,
        FAULT_9_TIME_HR = 619,
        FAULT_10_TIME_HR = 620,
        FAULT_1_TIME_MIN = 621,
        FAULT_2_TIME_MIN = 622,
        FAULT_3_TIME_MIN = 623,
        FAULT_4_TIME_MIN = 624,
        FAULT_5_TIME_MIN = 625,
        FAULT_6_TIME_MIN = 626,
        FAULT_7_TIME_MIN = 627,
        FAULT_8_TIME_MIN = 628,
        FAULT_9_TIME_MIN = 629,
        FAULT_10_TIME_MIN = 630,*/

        FAULT_1_FREQ = 631,
        FAULT_2_FREQ = 632,
        FAULT_3_FREQ = 633,
        FAULT_4_FREQ = 634,
        FAULT_5_FREQ = 635,
        FAULT_6_FREQ = 636,
        FAULT_7_FREQ = 637,
        FAULT_8_FREQ = 638,
        FAULT_9_FREQ = 639,
        FAULT_10_FREQ = 640,

        FAULT_1_CURR = 641,
        FAULT_2_CURR = 642,
        FAULT_3_CURR = 643,
        FAULT_4_CURR = 644,
        FAULT_5_CURR = 645,
        FAULT_6_CURR = 646,
        FAULT_7_CURR = 647,
        FAULT_8_CURR = 648,
        FAULT_9_CURR = 649,
        FAULT_10_CURR = 650,

        FAULT_1_BUS_VOLTS = 651,
        FAULT_2_BUS_VOLTS = 652,
        FAULT_3_BUS_VOLTS = 653,
        FAULT_4_BUS_VOLTS = 654,
        FAULT_5_BUS_VOLTS = 655,
        FAULT_6_BUS_VOLTS = 656,
        FAULT_7_BUS_VOLTS = 657,
        FAULT_8_BUS_VOLTS = 658,
        FAULT_9_BUS_VOLTS = 659,
        FAULT_10_BUS_VOLTS = 660,

        /*FAULT_1_STATUS = 661,
        FAULT_2_STATUS = 662,
        FAULT_3_STATUS = 663,
        FAULT_4_STATUS = 664,
        FAULT_5_STATUS = 665,
        FAULT_6_STATUS = 666,
        FAULT_7_STATUS = 667,
        FAULT_8_STATUS = 668,
        FAULT_9_STATUS = 669,
        FAULT_10_STATUS = 670,*/
    };

	/*DPIFaultObject::DPIFaultObject(CipUint instanceId, const SessionInfoIf::SPtr &si)
			: DPIFaultObject(instanceId, si, std::make_shared<MessageRouter>()) {

	}*/

	const DPIFaultObject::FullInformation &DPIFaultObject::getFullInformation() const {
		return _fullInformation;
	}

    const DPIFaultObject::FaultDetails &DPIFaultObject::getFaultDetails() const {
        return _faultDetails;
    }


	/*DPIFaultObject::DPIFaultObject(CipUint instanceId, const SessionInfoIf::SPtr &si,
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

			cip::CipUint flags;
			buffer >> flags;
			_fullInformation.isValidData = flags & VALID_DATA;
			_fullInformation.isRealTime = flags & REAL_TIME;
		} else {
			logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read FULL_INFORMATION attribute");
		}
	}*/


    static cip::CipLreal processVolts(uint16_t volts, int voltsParam)
    {
        auto * parameterObject = new ParameterObject(voltsParam, true, 4);

        parameterObject->setScalable(true);
        parameterObject->setScalingMultiplier(1);
        parameterObject->setScalingDivisor(1);
        parameterObject->setScalingBase(1);
        parameterObject->setScalingOffset(0);
        parameterObject->setPrecision(0);

        return parameterObject->actualToEngValue(volts);
    }

    static cip::CipLreal processCurrent(uint16_t current, int currentParam)
    {
        auto * parameterObject = new ParameterObject(currentParam, true, 4);

        parameterObject->setScalable(true);
        parameterObject->setScalingMultiplier(1);
        parameterObject->setScalingDivisor(1);
        parameterObject->setScalingBase(1);
        parameterObject->setScalingOffset(0);
        parameterObject->setPrecision(2);

        return parameterObject->actualToEngValue(current);
    }
    static cip::CipLreal processFrequency(uint16_t frequency, int frequencyParam)
    {
        auto * parameterObject = new ParameterObject(frequencyParam, true, 4);

        parameterObject->setScalable(true);
        parameterObject->setScalingMultiplier(1);
        parameterObject->setScalingDivisor(1);
        parameterObject->setScalingBase(1);
        parameterObject->setScalingOffset(0);
        parameterObject->setPrecision(2);

        return parameterObject->actualToEngValue(frequency);
    }


    /*
     * makes request to get info from fault parameters (volts, current & frequency)
     */
    static uint16_t getFaultDetail(const SessionInfoIf::SPtr &si,
                                                                   const MessageRouter::SPtr& messageRouter,
                                                                   int parameterNumber){

        auto response = messageRouter->sendRequest(si, ServiceCodes::GET_ATTRIBUTE_SINGLE,
                                                   EPath(0x0F, parameterNumber, 1));

        if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {

            Buffer buffer(response.getData());
            CipUint value;
            buffer >> value;

            return value;

        } else {
            logGeneralAndAdditionalStatus(response);
            throw std::runtime_error("Failed to read Get fault attributes");
        }
    }


	/*
	 * returns struct of data at time of fault (volts, current, frequency)
	 */
    DPIFaultObject::DPIFaultObject(const SessionInfoIf::SPtr &si,
                                   const MessageRouter::SPtr& messageRouter, int faultNumber, bool getFaultDetails)
            : BaseObject(0x0E, 0) {

        int faultCodeParam      = 0;
        int faultVoltsParam     = 0;
        int faultCurrentParam   = 0;
        int faultFrequencyParam = 0;

        switch(faultNumber){
            case 1:
                faultCodeParam = FaultParams::FAULT_1_CODE;
                faultVoltsParam = FaultParams::FAULT_1_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_1_CURR;
                faultFrequencyParam = FaultParams::FAULT_1_FREQ;
                break;
            case 2:
                faultCodeParam = FaultParams::FAULT_2_CODE;
                faultVoltsParam = FaultParams::FAULT_2_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_2_CURR;
                faultFrequencyParam = FaultParams::FAULT_2_FREQ;
                break;
            case 3:
                faultCodeParam = FaultParams::FAULT_3_CODE;
                faultVoltsParam = FaultParams::FAULT_3_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_3_CURR;
                faultFrequencyParam = FaultParams::FAULT_3_FREQ;
                break;
            case 4:
                faultCodeParam = FaultParams::FAULT_4_CODE;
                faultVoltsParam = FaultParams::FAULT_4_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_4_CURR;
                faultFrequencyParam = FaultParams::FAULT_4_FREQ;
                break;
            case 5:
                faultCodeParam = FaultParams::FAULT_5_CODE;
                faultVoltsParam = FaultParams::FAULT_5_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_5_CURR;
                faultFrequencyParam = FaultParams::FAULT_5_FREQ;
                break;
            case 6:
                faultCodeParam = FaultParams::FAULT_6_CODE;
                faultVoltsParam = FaultParams::FAULT_6_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_6_CURR;
                faultFrequencyParam = FaultParams::FAULT_6_FREQ;
                break;
            case 7:
                faultCodeParam = FaultParams::FAULT_7_CODE;
                faultVoltsParam = FaultParams::FAULT_7_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_7_CURR;
                faultFrequencyParam = FaultParams::FAULT_7_FREQ;
                break;
            case 8:
                faultCodeParam = FaultParams::FAULT_8_CODE;
                faultVoltsParam = FaultParams::FAULT_8_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_8_CURR;
                faultFrequencyParam = FaultParams::FAULT_8_FREQ;
                break;
            case 9:
                faultCodeParam = FaultParams::FAULT_9_CODE;
                faultVoltsParam = FaultParams::FAULT_9_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_9_CURR;
                faultFrequencyParam = FaultParams::FAULT_9_FREQ;
                break;
            case 10:
                faultCodeParam = FaultParams::FAULT_10_CODE;
                faultVoltsParam = FaultParams::FAULT_10_BUS_VOLTS;
                faultCurrentParam = FaultParams::FAULT_10_CURR;
                faultFrequencyParam = FaultParams::FAULT_10_FREQ;
                break;

            default:
                throw std::runtime_error("Invalid fault code (Fault code must be 1-10)");;
        }

        // get fault code from the device
        uint16_t faultCode = getFaultDetail(si, messageRouter, faultCodeParam);


        // there is a fault
        if(faultCode != 0){


            // get fault details (volts, current, & frequency)
            if(getFaultDetails){

                // get values from parameters
                uint16_t volts = getFaultDetail(si, messageRouter, faultVoltsParam);
                uint16_t current = getFaultDetail(si, messageRouter, faultCurrentParam);
                uint16_t frequency = getFaultDetail(si, messageRouter, faultFrequencyParam);


                // process volts, current & frequency to get engineering value
                _faultDetails = DPIFaultObject::FaultDetails {
                        faultNumber,
                        faultCode,
                        processVolts(volts, faultVoltsParam),
                        processCurrent(current, faultCurrentParam),
                        processFrequency(frequency, faultFrequencyParam),
                };

                return;
            }

        }


        // either no fault exists or don't want to get faults
        _faultDetails = DPIFaultObject::FaultDetails {
                faultNumber,
                faultCode,
                0,
                0,
                0,
        };
    }


    void DPIFaultObject::DPIFaultObject::setFaultDetails(DPIFaultObject::FaultDetails faultDetails)
    {
        _fullInformation.faultDetails = faultDetails;
    }

    void DPIFaultObject::DPIFaultObject::setFaultDescription(DPIFaultCode::FaultDescriptions faultDescriptions)
    {
        _fullInformation.faultDescription = faultDescriptions;
    }

}
}
}
}
