//
// Created by Aleksey Timin on 12/11/19.
//

#ifndef EIPSCANNER_DPIFAULTMANAGER_H
#define EIPSCANNER_DPIFAULTMANAGER_H

#include <vector>
#include <functional>
#include "DPIFaultObject.h"
#include "SessionInfoIf.h"

namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {

	enum DPIFaultManagerCommands : cip::CipUsint {
		NO_OPERATION = 0,
		CLEAR_FAULT = 1,
		CLEAR_FAULT_QUEUE = 2,
		RESET_DEVICE = 3
	};

	class DPIFaultManager {
	public:
		using NewFaultHandler = std::function<void(const DPIFaultObject& fault)>;
		using TrippedDeviceHandler = std::function<void(bool)>;

		DPIFaultManager();
		explicit DPIFaultManager(bool clearFaults, bool resetDevice, bool getFaultDetails);
		void setNewFaultListener(NewFaultHandler handler);
		void setTrippedDeviceListener(TrippedDeviceHandler handler);
		void handleFaultObjects(const SessionInfoIf::SPtr& si);
		void handleFaultObjects(const SessionInfoIf::SPtr& si, const MessageRouter::SPtr& messageRouter);

		void writeCommand(DPIFaultManagerCommands command, const SessionInfoIf::SPtr& si) const;
		void writeCommand(DPIFaultManagerCommands command, const SessionInfoIf::SPtr& si,
				const MessageRouter::SPtr& messageRouter) const;


	private:
		NewFaultHandler _newFaultHandler;
		TrippedDeviceHandler _trippedDeviceHandler;
		cip::CipUsint _lastTrippedState;
		bool _clearFaultsQueue; // clears fault queue after reading all fault
		bool _resetFault; // stops device after hard fault and restarts
		bool _getFaultDetails; // returns details at time of fault (frequency, current, voltage)
	};

}
}
}
}

#endif  // EIPSCANNER_DPIFAULTMANAGER_H
