//
// Created by Jan Ritzenhoff on 8/29/2024
//
#include "EIPScanner/vendor/teknic/clearlink/SDMotorInputObject.h"



#include "EIPScanner/utils/Buffer.h"

#include <type_traits>

namespace eipScanner
{
namespace vendor
{
namespace teknic
{
namespace clearlink
{
    using eipScanner::utils::Buffer;

    SDMotorInputObject::SDMotorInputObject(eipScanner::cip::CipUint instanceId, const eipScanner::SessionInfoIf::SPtr si, const eipScanner::MessageRouter::SPtr messageRouter)
        : AttributeBaseObject(CLASS_ID, instanceId, si, messageRouter), _commandedPosition(0), _commandedVelocity(0), _targetPosition(0), _targetVelocity(0), _capturedPosition(0), _measuredTorque(0), _motorStatus(0), _motorShutdowns(0)
    {

        _getAllAttributesIndividually();
    }

    std::vector<AttributeBaseObject::ReflexiveFieldReference> SDMotorInputObject::_getAttributeReferences()
    {
        return {std::ref(_commandedPosition), std::ref(_commandedVelocity), std::ref(_targetPosition),
                std::ref(_targetVelocity), std::ref(_capturedPosition), std::ref(_measuredTorque),
                std::ref(_motorStatus), std::ref(_motorShutdowns)};
    }

    eipScanner::cip::CipDint SDMotorInputObject::getCommandedPosition() const { return _commandedPosition; }
    eipScanner::cip::CipDint SDMotorInputObject::getCommandedVelocity() const { return _commandedVelocity; }
    eipScanner::cip::CipDint SDMotorInputObject::getTargetPosition() const { return _targetPosition; }
    eipScanner::cip::CipDint SDMotorInputObject::getTargetVelocity() const { return _targetVelocity; }
    eipScanner::cip::CipDint SDMotorInputObject::getCapturedPosition() const { return _capturedPosition; }
    eipScanner::cip::CipReal SDMotorInputObject::getMeasuredTorque() const { return _measuredTorque; }
    eipScanner::cip::CipDword SDMotorInputObject::getMotorStatus() const { return _motorStatus; }
    eipScanner::cip::CipDword SDMotorInputObject::getMotorShutdowns() const { return _motorShutdowns; }

    void SDMotorInputObject::printMotorInputState(eipScanner::utils::Logger logger)
    {
        eipScanner::cip::CipDword lastMotorStatus = getMotorStatus();

        for (int stateBit = InputState::AtTargetPosition; stateBit < InputState::NUM_STATES; ++stateBit)
        {
            // Check if the ith bit is set
            if (lastMotorStatus & (1 << stateBit))
            {
                // Print the corresponding state
                logger << "State " << stateToString(InputState(stateBit)) << " is set." << std::endl;
            }
        }
    }
}
}
}
}
