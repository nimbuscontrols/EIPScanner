//
// Created by Aleksey Timin on 12/11/19.
//

#include "utils/Buffer.h"
#include "utils/Logger.h"
#include "DPIFaultManager.h"

namespace eipScanner {
namespace vendor {
namespace ra {
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

	DPIFaultManager::DPIFaultManager()
		: _newFaultHandler([](auto){})
		, _trippedDeviceHandler([](auto){})
		, _lastTrippedState(-1) {
	}

	void DPIFaultManager::setNewFaultListener(DPIFaultManager::NewFaultHandler handler) {
		_newFaultHandler = std::move(handler);
	}

	void DPIFaultManager::setTrippedDeviceListener(DPIFaultManager::TrippedDeviceHandler handler) {
		_trippedDeviceHandler = std::move(handler);
	}

	void
	DPIFaultManager::handleFaultObjects(const SessionInfoIf::SPtr &si, const MessageRouter::SPtr &messageRouter) {
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
				Logger(LogLevel::INFO) << "There are " << faultNumber << " faults in the queue";

				for (int i = 1; i <= faultNumber; ++i) {
					_newFaultHandler(DPIFaultObject(i, si, messageRouter));
				}

				Logger(LogLevel::INFO) << "All faults are read. Clean the queue";
				writeCommand(DPIFaultManagerCommands::CLEAR_FAULT_QUEUE, si, messageRouter);
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