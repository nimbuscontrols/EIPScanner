//
// Created by James Roth on 12/18/19.
//

#include "EIPScanner/vendor/ra/powerFlex525/DPIFaultCode.h"


namespace eipScanner {
namespace vendor {
namespace ra {
namespace powerFlex525 {


	/*
	 * constructor
	 */
	DPIFaultCode::DPIFaultCode::DPIFaultCode(int faultCode) {
		this->_faultCode = faultCode;
	}

	/*
	 * destructor
	 */
	DPIFaultCode::~DPIFaultCode() {}


	/*
	 * returns text related to fault
	 */
	DPIFaultCode::FaultDescriptions DPIFaultCode::getFaultDescription() {

		// make sure map contains faultCode - tried using try/catch but wasn't catching my exception
		if (this->containsFaultCode())
			return this->_faultMap.at(this->_faultCode);
		else
			throw std::runtime_error("Fault code doesn't exist for Powerflex 525.");

	}


	/*
	 * return bool whether or not the fault code exists in map
	 */
	bool DPIFaultCode::containsFaultCode()
	{
		// doesn't contain fault
		if (this->_faultMap.find(this->_faultCode) == this->_faultMap.end())
			return false;
		else // contains fault
			return true;
	}


}
}
}
}