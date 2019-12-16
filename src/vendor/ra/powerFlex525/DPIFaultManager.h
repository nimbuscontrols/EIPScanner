//
// Created by Aleksey Timin on 12/11/19.
//

#ifndef EIPSCANNER_DPIFAULTMANAGER_H
#define EIPSCANNER_DPIFAULTMANAGER_H

#include <functional>
#include "DPIFaultObject.h"
#include "SessionInfoIf.h"

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
	 */
	class DPIFaultManager {
	public:
		using NewFaultHandler = std::function<void(const DPIFaultObject& fault)>;
		using TrippedDeviceHandler = std::function<void(bool)>;

		/**
		 * @brief Default constructor (clearFaults = true)
		 */
		DPIFaultManager();

		/**
		 * @brief Constructor
		 * @param clearFaults if true the manager clears the queue after it has retrieved a new fault
		 */
		explicit DPIFaultManager(bool clearFaults);

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
		 * @brief Checks if there are new faults in the queue
		 * @param si the EIP session for explicit messaging
		 */
		void handleFaultObjects(const SessionInfoIf::SPtr& si);

		/**
		 * @note used fot testing
		 * @param si
		 * @param messageRouter
		 */
		void handleFaultObjects(const SessionInfoIf::SPtr& si, const MessageRouter::SPtr& messageRouter);

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
		bool _clearFaults;
	};

}
}
}
}

#endif  // EIPSCANNER_DPIFAULTMANAGER_H
