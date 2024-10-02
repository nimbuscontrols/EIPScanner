//
// Created by Jan Ritzenhoff on 8/29/2024
//

#include "EIPScanner/vendor/teknic/clearlink/SDMotorOutputObject.h"

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

    SDMotorOutputObject::SDMotorOutputObject(eipScanner::cip::CipUint instanceId, const eipScanner::SessionInfoIf::SPtr si, const eipScanner::MessageRouter::SPtr messageRouter)
        : AttributeBaseObject(CLASS_ID, instanceId, si, messageRouter), _moveDistance(0), _jogVelocity(0), _velocityLimitForPositionalMove(0), _accelerationLimitForPositionalMove(0), _decelerationLimit(0), _outputRegister(0), _addToPosition(0)
    {

        _getAllAttributesIndividually();
    }

    void SDMotorOutputObject::setAllAttributesIndividually()
    {
        _setAllAttributesIndividually();
    }

    std::vector<AttributeBaseObject::ReflexiveFieldReference> SDMotorOutputObject::_getAttributeReferences()
    {
        return {std::ref(_moveDistance), std::ref(_jogVelocity), std::ref(_velocityLimitForPositionalMove), std::ref(_accelerationLimitForPositionalMove),
                std::ref(_decelerationLimit), std::ref(_outputRegister), std::ref(_addToPosition)};
    }

    eipScanner::cip::CipDint SDMotorOutputObject::getMoveDistance() const { return _moveDistance; }
    void SDMotorOutputObject::setMoveDistance(eipScanner::cip::CipDint moveDistance) { _moveDistance = moveDistance; }

    eipScanner::cip::CipDint SDMotorOutputObject::getJogVelocity() const { return _jogVelocity; }
    void SDMotorOutputObject::setJogVelocity(eipScanner::cip::CipDint jogVelocity) { _jogVelocity = jogVelocity; }

    eipScanner::cip::CipUdint SDMotorOutputObject::getVelocityLimitForPositionalMove() const { return _velocityLimitForPositionalMove; }
    void SDMotorOutputObject::setVelocityLimitForPositionalMove(eipScanner::cip::CipUdint velocityLimitForPositionalMove)
    {
        _velocityLimitForPositionalMove = velocityLimitForPositionalMove;
    }

    eipScanner::cip::CipUdint SDMotorOutputObject::getAccelerationLimitForPositionalMove() const { return _accelerationLimitForPositionalMove; }
    void SDMotorOutputObject::setAccelerationLimitForPositionalMove(eipScanner::cip::CipUdint accelerationLimitForPositionalMove)
    {
        _accelerationLimitForPositionalMove = accelerationLimitForPositionalMove;
    }

    eipScanner::cip::CipUdint SDMotorOutputObject::getDecelerationLimit() const { return _decelerationLimit; }
    void SDMotorOutputObject::setDecelerationLimit(eipScanner::cip::CipUdint decelerationLimit) { _decelerationLimit = decelerationLimit; }

    eipScanner::cip::CipDword SDMotorOutputObject::getOutputRegister() const { return _outputRegister; }
    void SDMotorOutputObject::setOutputRegister(eipScanner::cip::CipDword outputRegister) { _outputRegister = outputRegister; }

    eipScanner::cip::CipDint SDMotorOutputObject::getAddToPosition() const { return _addToPosition; }
    void SDMotorOutputObject::setAddToPosition(eipScanner::cip::CipDint addToPosition) { _addToPosition = addToPosition; }

}
}
}
}
