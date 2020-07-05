//
// Created by James Roth on 12/18/19.
//

#ifndef EIPSCANNER_DPIFAULTCODES_HPP
#define EIPSCANNER_DPIFAULTCODES_HPP

#include <stdexcept>
#include <unordered_map>
#include <string>

using namespace std;

namespace eipScanner {
	namespace vendor {
		namespace ra {
			namespace powerFlex525 {

				class DPIFaultCode {

				public:

					explicit DPIFaultCode(int faultCode);
					~DPIFaultCode();

					struct FaultDescriptions {
						int faultType;
						string faultText;
						string faultDescription;
					};

					FaultDescriptions getFaultDescription(); // returns FaultDescriptions struct{} related to fault

				private:

					bool containsFaultCode(); // return bool whether or not the fault code exists in map

					int _faultCode; // fault code from the device


					// stores all fault info in map <faultCode, description> - https://literature.rockwellautomation.com/idc/groups/literature/documents/du/520-du001_-en-e.pdf - page 5
					unordered_map<int, FaultDescriptions> _faultMap = {

							{0, FaultDescriptions{
									0,
									"No Fault",
									"No fault present."
							}},
							{2, FaultDescriptions{
									1,
									"Heatsink OvrTmp",
									"Heatsink/Power Module temperature exceeds a predefined value."
							}},
							{3, FaultDescriptions{
									2,
									"Power Loss",
									"Single phase operation detected with excessive load."
							}},
							{4, FaultDescriptions{
									1,
									"UnderVoltage",
									"DC bus voltage fell below the minimum value."
							}},
							{5, FaultDescriptions{
									1,
									"OverVoltage",
									"DC bus voltage exceeded maximum value."
							}},
							{6, FaultDescriptions{
									1,
									"Motor Stalled",
									"Drive is unable to accelerate or decelerate motor."
							}},
							{7, FaultDescriptions{
									1,
									"Motor Overload",
									"Internal electronic overload trip."
							}},
							{8, FaultDescriptions{
									1,
									"Heatsink OvrTmp",
									"Heatsink/Power Module temperature exceeds a predefined value."
							}},
							{9, FaultDescriptions{
									1,
									"CC OvrTmp",
									"Control module temperature exceeds a predefined value."
							}},
							{12, FaultDescriptions{
									2,
									"HW OverCurrent",
									"The drive output current has exceeded the hardware current limit."
							}},
							{13, FaultDescriptions{
									1,
									"Ground Fault",
									"A current path to earth ground has been detected at one or more of the drive output terminals."
							}},
							{15, FaultDescriptions{
									2,
									"Load Loss",
									"The output torque current is below the value programmed in A490 [Load Loss Level] for a time period greater than the time programmed in A491 [Load Loss Time]."
							}},
							{21, FaultDescriptions{
									1,
									"Output Ph Loss",
									"Output Phase Loss (if enabled). Configure with A557 [Output Phas Loss En]"
							}},
							{29, FaultDescriptions{
									1,
									"Analog In Loss",
									"An analog input is configured to fault on signal loss. A signal loss has occurred. Configure with t094 [Anlg In V Loss] or t097 [Anlg In mA Loss]."
							}},
							{33, FaultDescriptions{
									2,
									"Auto Rstrt Tries",
									"Drive unsuccessfully attempted to reset a fault and resume running for the programmed number of A541 [Auto Rstrt Tries]."
							}},
							{38, FaultDescriptions{
									2,
									"Phase U to Gnd",
									"A phase to ground fault has been detected between the drive and motor in this phase."
							}},
							{39, FaultDescriptions{
									2,
									" Phase V to Gnd",
									"A phase to ground fault has been detected between the drive and motor in this phase."
							}},
							{40, FaultDescriptions{
									2,
									" Phase W to Gnd",
									"A phase to ground fault has been detected between the drive and motor in this phase."
							}},
							{41, FaultDescriptions{
									2,
									"Phase UV Short",
									"Excessive current has been detected between these two output terminals"
							}},
							{42, FaultDescriptions{
									2,
									"Phase UW Short",
									"Excessive current has been detected between these two output terminals"
							}},
							{43, FaultDescriptions{
									2,
									"Phase VW Short",
									"Excessive current has been detected between these two output terminals"
							}},
							{48, FaultDescriptions{
									1,
									"Params Defaulted",
									" The drive was commanded to write default values to EEPROM."
							}},
							{59, FaultDescriptions{
									1,
									"Safety Open",
									"Both of the safety inputs (Safety 1, Safety 2) are not enabled. Configure with t105 [Safety Open En]."
							}},
							{63, FaultDescriptions{
									1,
									"SW OverCurrent",
									"Programmed A486, A488 [Shear Pinx Level] has been exceeded for a time period greater than the time programmed in A487, A489 [Shear Pin x Time]."
							}},
							{64, FaultDescriptions{
									2,
									"Drive Overload",
									"Drive overload rating has been exceeded."
							}},
							{70, FaultDescriptions{
									2,
									"Power Unit",
									"Failure has been detected in the drive power section."
							}},
							{71, FaultDescriptions{
									2,
									"DSI Net Loss",
									"Control over the Modbus or DSI communications link has been interrupted."
							}},
							{72, FaultDescriptions{
									2,
									"Opt Net Loss",
									"Control over the network option card’s remote network has been interrupted."
							}},
							{73, FaultDescriptions{
									2,
									"EN Net Loss",
									"Control through the embedded EtherNet/IP adapter has been interrupted."
							}},
							{80, FaultDescriptions{
									2,
									"Autotune Failure",
									"The autotune function was either cancelled by the user or failed."
							}},
							{81, FaultDescriptions{
									2,
									"DSI Comm Loss",
									"Communications between the drive and the Modbus or DSI master device have been interrupted."
							}},
							{82, FaultDescriptions{
									2,
									"Opt Comm Loss",
									"Communications between the drive and the network option card have been interrupted."
							}},
							{83, FaultDescriptions{
									2,
									"EN Comm Loss",
									"Internal communications between the drive and the embedded EtherNet/IP adapter have been interrupted."
							}},
							{91, FaultDescriptions{
									2,
									"Encoder Loss",
									"Requires differential encoder. One of the 2 encoder channel signals is missing"
							}},
							{94, FaultDescriptions{
									2,
									"Function Loss",
									"'Freeze-Fire' (Function Loss) input is inactive, input to the programmed terminal is open."
							}},
							{100, FaultDescriptions{
									2,
									"Parameter Chksum",
									"Drive parameter non-volatile storage is corrupted."
							}},
							{101, FaultDescriptions{
									2,
									"External Storage",
									"External non-volatile storage has failed."
							}},
							{105, FaultDescriptions{
									2,
									"C Connect Err",
									"Control module was disconnected while drive was powered."
							}},
							{106, FaultDescriptions{
									2,
									"Incompat C-P",
									"The PowerFlex 525 control module does not support power modules with 0.25 HP power rating"
							}},
							{107, FaultDescriptions{
									2,
									"Replaced C-P",
									"The control module could not recognize the power module. Hardware failure."
							}},
							{109, FaultDescriptions{
									2,
									"Mismatch C-P",
									"The control module was mounted to a different drive type power module"
							}},
							{110, FaultDescriptions{
									2,
									"Keypad Membrane",
									"Keypad membrane failure / disconnected."
							}},
							{111, FaultDescriptions{
									2,
									"Safety Hardware",
									"Safety input enable hardware malfunction. One of the safety inputs is not enabled."
							}},
							{114, FaultDescriptions{
									2,
									"uC Failure",
									"Microprocessor failure."
							}},
							{122, FaultDescriptions{
									2,
									"I/O Board Fail",
									"Failure has been detected in the drive control and I/O section."
							}},
							{125, FaultDescriptions{
									2,
									"Flash Update Req",
									"The firmware in the drive is corrupt, mismatched, or incompatible with the hardware."
							}},
							{126, FaultDescriptions{
									2,
									"NonRecoverablErr",
									"A non-recoverable firmware or hardware error was detected. The drive was automatically stopped and reset."
							}},
							{127, FaultDescriptions{
									2,
									"DSIFlashUpdatReq",
									"A critical problem with the firmware was detected and the drive is running using backup firmware that only supports DSI communications."
							}},
					};

				};

			}

		}
	}
}


#endif //EIPSCANNER_DPIFAULTCODES_HPP
