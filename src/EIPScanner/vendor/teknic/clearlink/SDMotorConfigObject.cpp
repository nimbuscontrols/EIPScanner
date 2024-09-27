//
// Created by Jan Ritzenhoff on 8/28/2024
//
#include "EIPScanner/vendor/teknic/clearlink/SDMotorConfigObject.h"

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

    SDMotorConfigObject::SDMotorConfigObject(eipScanner::cip::CipUint instanceId, const eipScanner::SessionInfoIf::SPtr si, const eipScanner::MessageRouter::SPtr messageRouter)
        : AttributeBaseObject(CLASS_ID, instanceId, si, messageRouter), _configRegister(0), _softLimitPosition1(0), _softLimitPosition2(0), _positiveLimitConnector(0), _negativeLimitConnector(0), _homeSensorConnector(0), _brakeOutputConnector(0), _triggerPositionCaptureConnector(0), _maxDecelerationRate(0), _stopSensorConnector(0), _followEncoderMode(0), _followDivisor(0), _followMultiplier(0)
    {

        _getAllAttributesIndividually();
    }

    void SDMotorConfigObject::setAllAttributesIndividually()
    {
        _setAllAttributesIndividually();
    }

    std::vector<AttributeBaseObject::ReflexiveFieldReference> SDMotorConfigObject::_getAttributeReferences()
    {
        return {std::ref(_configRegister), std::ref(_softLimitPosition1), std::ref(_softLimitPosition2),
                std::ref(_positiveLimitConnector), std::ref(_negativeLimitConnector), std::ref(_homeSensorConnector),
                std::ref(_brakeOutputConnector), std::ref(_triggerPositionCaptureConnector), std::ref(_maxDecelerationRate),
                std::ref(_stopSensorConnector), std::ref(_followEncoderMode), std::ref(_followDivisor), std::ref(_followMultiplier)};
    }

    eipScanner::cip::CipDword SDMotorConfigObject::getConfigRegister() const { return _configRegister; }
    void SDMotorConfigObject::setConfigRegister(eipScanner::cip::CipDword configRegister) { _configRegister = configRegister; }

    eipScanner::cip::CipDint SDMotorConfigObject::getSoftLimitPosition1() const { return _softLimitPosition1; }
    void SDMotorConfigObject::setSoftLimitPosition1(eipScanner::cip::CipDint softLimitPosition1) { _softLimitPosition1 = softLimitPosition1; }

    eipScanner::cip::CipDint SDMotorConfigObject::getSoftLimitPosition2() const { return _softLimitPosition2; }
    void SDMotorConfigObject::setSoftLimitPosition2(eipScanner::cip::CipDint softLimitPosition2) { _softLimitPosition2 = softLimitPosition2; }

    eipScanner::cip::CipSint SDMotorConfigObject::getPositiveLimitConnector() const { return _positiveLimitConnector; }
    void SDMotorConfigObject::setPositiveLimitConnector(eipScanner::cip::CipSint positiveLimitConnector) { _positiveLimitConnector = positiveLimitConnector; }

    eipScanner::cip::CipSint SDMotorConfigObject::getNegativeLimitConnector() const { return _negativeLimitConnector; }
    void SDMotorConfigObject::setNegativeLimitConnector(eipScanner::cip::CipSint negativeLimitConnector) { _negativeLimitConnector = negativeLimitConnector; }

    eipScanner::cip::CipSint SDMotorConfigObject::getHomeSensorConnector() const { return _homeSensorConnector; }
    void SDMotorConfigObject::setHomeSensorConnector(eipScanner::cip::CipSint homeSensorConnector) { _homeSensorConnector = homeSensorConnector; }

    eipScanner::cip::CipSint SDMotorConfigObject::getBrakeOutputConnector() const { return _brakeOutputConnector; }
    void SDMotorConfigObject::setBrakeOutputConnector(eipScanner::cip::CipSint brakeOutputConnector) { _brakeOutputConnector = brakeOutputConnector; }

    eipScanner::cip::CipSint SDMotorConfigObject::getTriggerPositionCaptureConnector() const { return _triggerPositionCaptureConnector; }
    void SDMotorConfigObject::setTriggerPositionCaptureConnector(eipScanner::cip::CipSint triggerPositionCaptureConnector) { _triggerPositionCaptureConnector = triggerPositionCaptureConnector; }

    eipScanner::cip::CipDint SDMotorConfigObject::getMaxDecelerationRate() const { return _maxDecelerationRate; }
    void SDMotorConfigObject::setMaxDecelerationRate(eipScanner::cip::CipDint maxDecelerationRate) { _maxDecelerationRate = maxDecelerationRate; }

    eipScanner::cip::CipSint SDMotorConfigObject::getStopSensorConnector() const { return _stopSensorConnector; }
    void SDMotorConfigObject::setStopSensorConnector(eipScanner::cip::CipSint stopSensorConnector) { _stopSensorConnector = stopSensorConnector; }

    eipScanner::cip::CipSint SDMotorConfigObject::getFollowEncoderMode() const { return _followEncoderMode; }
    void SDMotorConfigObject::setFollowEncoderMode(eipScanner::cip::CipSint followEncoderMode) { _followEncoderMode = followEncoderMode; }

    eipScanner::cip::CipDint SDMotorConfigObject::getFollowDivisor() const { return _followDivisor; }
    void SDMotorConfigObject::setFollowDivisor(eipScanner::cip::CipDint followDivisor) { _followDivisor = followDivisor; }

    eipScanner::cip::CipDint SDMotorConfigObject::getFollowMultiplier() const { return _followMultiplier; }
    void SDMotorConfigObject::setFollowMultiplier(eipScanner::cip::CipDint followMultiplier) { _followMultiplier = followMultiplier; }
}
}
}
}
