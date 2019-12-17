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
		auto response = messageRouter->sendRequest(si,
				ServiceCodes::GET_ATTRIBUTE_SINGLE,
				EPath(DPIFaultObject::CLASS_ID, 0 , DPIFaultClassAttributeIds::FAULT_TRIP_INSTANCE_READ));

		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			Buffer buffer(response.getData());
			CipUsint tripped;
			buffer >> tripped;

			if (_lastTrippedState != tripped) {
				Logger(LogLevel::INFO) << "Trip flag changed to " << static_cast<int>(tripped);
				_trippedDeviceHandler(tripped);
				_lastTrippedState = tripped;
			}
		} else {
			logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read FAULT_TRIP_INSTANCE_READ attribute");
		}

		response = messageRouter->sendRequest(si,
				ServiceCodes::GET_ATTRIBUTE_SINGLE,
				EPath(DPIFaultObject::CLASS_ID, 0 , DPIFaultClassAttributeIds::NUMBER_OF_RECORDED_FAULTS));

		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			Buffer buffer(response.getData());
			CipUint faultNumber;
			buffer >> faultNumber;

			if (faultNumber > 0) {
				int realFaultCount = 0;

				for (int i = 1; i <= faultNumber; ++i) {
					DPIFaultObject faultObject(i, si, messageRouter);
					if (faultObject.getFullInformation().faultCode == 0) {
						break;
					}

					// i think we should clear the queue after the loop verses clearing 1 at a time
					/*if (_clearFaultsQueue) {
						writeCommand(DPIFaultManagerCommands::CLEAR_FAULT, si, messageRouter);
					}*/


                    // get details at time of fault (volts, current & frequency) and add to faultInformation
					if(_getFaultDetails) {
                        auto faultInformation = new DPIFaultObject(si, messageRouter, i);
                        auto faultDetails = faultInformation->getFaultDetails();
                        faultObject.setFaultDetails(faultDetails);
                    }


					realFaultCount++;
					_newFaultHandler(faultObject);
				}

				if (realFaultCount > 0) {
					Logger(LogLevel::INFO) << "There read " << realFaultCount << " faults in the queue";
				}


				// we should clear this in the end after the loop
                if (_clearFaultsQueue && realFaultCount > 0) {
                    writeCommand(DPIFaultManagerCommands::CLEAR_FAULT_QUEUE, si, messageRouter);
                }

                // device is stopped -> need to figure out how to 1) read fault that stops device and 2) reset device
                if(_resetFault && realFaultCount > 0){

                }
			}
		} else {
			logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read NUMBER_OF_RECORDED_FAULTS attribute");
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
