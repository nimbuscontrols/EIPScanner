//
// Created by James Roth on 12/18/19.
//

#include "DPIFaultCode.h"


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
        if (this->containsFaultCode()){
            return this->_faultMap.at(this->_faultCode);
        } else{
            // faultCode doesn't exists
            return FaultDescriptions{
                    -1,
                    "n/a",
                    "n/a"
            };
        }
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