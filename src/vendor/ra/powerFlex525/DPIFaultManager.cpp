//
// Created by Aleksey Timin on 12/11/19.
//

#include "utils/Buffer.h"
#include "utils/Logger.h"
#include "DPIFaultManager.h"

namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {

	using namespace cip;
	using utils::Buffer;
	using utils::LogLevel;
	using utils::Logger;

	enum DPIFaultClassAttributeIds : CipUsint {
		CLASS_REVISION = 1,
		NUMBER_OF_INSTANCE = 2,
		FAULT_COMMAND_WRITE = 3,
		FAULT_TRIP_INSTANCE_READ = 4,
		FAULT_DATA_LIST = 5,
		NUMBER_OF_RECORDED_FAULTS = 6
	};

	DPIFaultManager::DPIFaultManager() : DPIFaultManager(true, false, false) {
	}

	DPIFaultManager::DPIFaultManager(bool clearFaults, bool resetDevice, bool getFaultDetails)
		: _newFaultHandler([](auto){})
		, _trippedDeviceHandler([](auto){})
		, _lastTrippedState(-1)
		, _clearFaultsQueue(clearFaults)
		, _resetFault(resetDevice)
		, _getFaultDetails(getFaultDetails){
	}

	void DPIFaultManager::setNewFaultListener(DPIFaultManager::NewFaultHandler handler) {
		_newFaultHandler = std::move(handler);
	}

	void DPIFaultManager::setTrippedDeviceListener(DPIFaultManager::TrippedDeviceHandler handler) {
		_trippedDeviceHandler = std::move(handler);
	}

	void DPIFaultManager::handleFaultObjects(const SessionInfoIf::SPtr &si, const MessageRouter::SPtr &messageRouter) {

		uint16_t faultCount = 0; // keeps track of number of faults we count in the parameters

		// up to 10 faults in powerflex 525
        for (int i = 1; i <= 10; ++i) {


            // check if fault exists and if fault, return fault info
            auto faultInformation = DPIFaultObject(si, messageRouter, i, _getFaultDetails);
            auto faultDetails = faultInformation.getFaultDetails();


            // no fault
            if(faultDetails.faultCode == 0)
                break;
            else {

                // get fault descriptions mapped from fault code)
                auto faultCodes = DPIFaultCode(faultDetails.faultCode).getFaultDescription();
                faultInformation.setFaultDescription(faultCodes);
                faultInformation.setFaultDetails(faultDetails);
            }


            faultCount++;
            _newFaultHandler(faultInformation);
        }


        if (faultCount > 0) {
            Logger(LogLevel::INFO) << "There read " << faultCount << " faults in the queue";
        }


        // we should clear this in the end after the loop
        if (_clearFaultsQueue && faultCount > 0) {
            writeCommand(DPIFaultManagerCommands::CLEAR_FAULT_QUEUE, si, messageRouter);
        }

        // device is stopped -> need to figure out how to 1) read fault that stops device and 2) reset device
        if(_resetFault && faultCount > 0){

        }
	}

	void DPIFaultManager::handleFaultObjects(const SessionInfoIf::SPtr &si) {
		handleFaultObjects(si, std::make_shared<MessageRouter>());
	}

	void DPIFaultManager::writeCommand(DPIFaultManagerCommands command, const SessionInfoIf::SPtr &si) const {
		writeCommand(command, si, std::make_shared<MessageRouter>());
	}

	void DPIFaultManager::writeCommand(DPIFaultManagerCommands command, const SessionInfoIf::SPtr &si,
									   const MessageRouter::SPtr &messageRouter) const {

		Buffer buffer;
		buffer << static_cast<CipUsint>(command);
		auto response = messageRouter->sendRequest(si,
				ServiceCodes::SET_ATTRIBUTE_SINGLE,
				EPath(DPIFaultObject::CLASS_ID, 0 , DPIFaultClassAttributeIds::FAULT_COMMAND_WRITE),
				buffer.data());

		if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
			logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to send command");
		}
	}
}
}
}
}
