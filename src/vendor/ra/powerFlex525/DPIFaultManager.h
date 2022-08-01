//
// Created by Aleksey Timin on 12/11/19.
//

#ifndef EIPSCANNER_DPIFAULTMANAGER_H
#define EIPSCANNER_DPIFAULTMANAGER_H

#include <vector>
#include <functional>

#include "DPIFaultObject.h"
#include "SessionInfoIf.h"
#include "DPIFaultParameter.h"

namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {

	/**
	 * Fault Manager command codes
	 */
	enum DPIFaultManagerCommands : cip::CipUsint {
		NO_OPERATION = 0,
		CLEAR_FAULT = 1,
		CLEAR_FAULT_QUEUE = 2,
		RESET_DEVICE = 3
	};

	/**
	 * @class DPIFaultManager
	 *
	 * @brief Implements a manager to retrieve new faults and clean its queue
	 *
	 * It use PrarameterObejcts instead of FaultObject because it doesn't contain the needed information
	 *
	 */
	class DPIFaultManager {
	public:
		using NewFaultObjectHandler = std::function<void(const DPIFaultObject& fault)>;
		using NewFaultHandler = std::function<void(const DPIFaultParameter& fault)>;
		using TrippedDeviceHandler = std::function<void(bool)>;

		/**
		 * @brief Default constructor (clearFaults = true, resetDevice = false, getFaultDetails = false)
		 */
		DPIFaultManager();
  
		/**
		 * @brief Constructor
		 * @param clearFaults if true the manager clears the queue after it has retrieved a new fault
		 * @param resetDevice isn't used yet
		 * @param getFaultDetails if true the manager read all data from fault parameters
		 */
		explicit DPIFaultManager(bool clearFaults, bool resetDevice, bool getFaultDetails);

		/**
		 * @brief Sets a callback to receive a new fault
		 * @param handler
		 */
		void setNewFaultListener(NewFaultHandler handler);

		/**
		 * @brief Sets a callback if the device changed trip-state
		 * @param handler
		 */
		void setTrippedDeviceListener(TrippedDeviceHandler handler);

		/**
		 * @brief reads fault parameters and calls NewFaultHandler handler if it gets a new one
		 * @param si
		 */
		void handleFaultParameters(const SessionInfoIf::SPtr& si);
		void handleFaultParameters(const SessionInfoIf::SPtr& si, const MessageRouter::SPtr& messageRouter);

		/**
		 * @brief Writs a command to DPI Fault Manager (e.g. clean fault or reset device) @sa DPIFaultManagerCommands
		 * @param command the command to send
		 * @param si the EIP session for explicit messaging
		 */
		void writeCommand(DPIFaultManagerCommands command, const SessionInfoIf::SPtr& si) const;

		/**
		 * @note used for testing
		 * @param command
		 * @param si
		 * @param messageRouter
		 */
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
