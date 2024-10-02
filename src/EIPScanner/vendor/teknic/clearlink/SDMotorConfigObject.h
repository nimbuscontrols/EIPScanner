//
// Created by Jan Ritzenhoff on 8/28/2024
//

// Based on:
//	Step and Direction Motor Configuration Object (64hex - 4 instances)
//		* https://www.teknic.com/files/downloads/clearlink_ethernet-ip_object_reference.pdf#page=37

#ifndef EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTORCONFIGOBJECT_H
#define EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTORCONFIGOBJECT_H

#include "EIPScanner/cip/CipRevision.h"
#include "EIPScanner/cip/CipString.h"

#include "EIPScanner/vendor/teknic/clearlink/AttributeBaseObject.h"


namespace eipScanner
{
namespace vendor
{
namespace teknic
{
namespace clearlink
{
    /**
     * @class SDMotorConfigObject
     *
     * @brief Implements interface to Identity Object (0x64).
     *
     * It reads all data from the CIP instance in the constructor
     */
    class SDMotorConfigObject : public AttributeBaseObject
    {
    public:
        static const eipScanner::cip::CipUint CLASS_ID = 0x64;

        /**
         * @brief Creates an instance and reads all its data via EIP
         * @note Used for testing
         * @param instanceId
         * @param si
         * @param messageRouter
         */
        SDMotorConfigObject(eipScanner::cip::CipUint instanceId, const eipScanner::SessionInfoIf::SPtr si, const eipScanner::MessageRouter::SPtr messageRouter);

        /**
         * @brief Gets Config Register [AttrID=1]
         * @return
         */
        eipScanner::cip::CipDword getConfigRegister() const;

        /**
         * @brief Gets SoftLimitPosition1 [AttrID=2]
         * @return
         */
        eipScanner::cip::CipDint getSoftLimitPosition1() const;

        /**
         * @brief Gets SoftLimitPosition2 [AttrID=3]
         * @return
         */
        eipScanner::cip::CipDint getSoftLimitPosition2() const;

        /**
         * @brief Gets PositiveLimitConnector [AttrID=4]
         * @return
         */
        eipScanner::cip::CipSint getPositiveLimitConnector() const;

        /**
         * @brief Gets NegativeLimitConnector [AttrID=5]
         * @return
         */
        eipScanner::cip::CipSint getNegativeLimitConnector() const;

        /**
         * @brief Gets HomeSensorConnector [AttrID=6]
         * @return
         */
        eipScanner::cip::CipSint getHomeSensorConnector() const;

        /**
         * @brief Gets BrakeOutputConnector [AttrID=7]
         * @return
         */
        eipScanner::cip::CipSint getBrakeOutputConnector() const;

        /**
         * @brief Gets TriggerPositionCaptureConnector [AttrID=8]
         * @return
         */
        eipScanner::cip::CipSint getTriggerPositionCaptureConnector() const;

        /**
         * @brief Gets MaxDecelerationRate [AttrID=9]
         * @return
         */
        eipScanner::cip::CipDint getMaxDecelerationRate() const;

        /**
         * @brief Gets StopSensorConnector [AttrID=10]
         * @return
         */
        eipScanner::cip::CipSint getStopSensorConnector() const;

        /**
         * @brief Gets FollowEncoderMode [AttrID=11]
         * @return
         */
        eipScanner::cip::CipSint getFollowEncoderMode() const;

        /**
         * @brief Gets FollowDivisor [AttrID=12]
         * @return
         */
        eipScanner::cip::CipDint getFollowDivisor() const;

        /**
         * @brief Gets FollowMultiplier [AttrID=13]
         * @return
         */
        eipScanner::cip::CipDint getFollowMultiplier() const;

        // -------------------------------------------------------------

        void setAllAttributesIndividually();

        // -------------------------------------------------------------

        /**
         * @brief Sets Config Register [AttrID=1]
         * @return
         */
        void setConfigRegister(eipScanner::cip::CipDword configRegister);

        /**
         * @brief Sets SoftLimitPosition1 [AttrID=2]
         * @return
         */
        void setSoftLimitPosition1(eipScanner::cip::CipDint softLimitPosition1);

        /**
         * @brief Sets SoftLimitPosition2 [AttrID=3]
         * @return
         */
        void setSoftLimitPosition2(eipScanner::cip::CipDint softLimitPosition2);

        /**
         * @brief Sets PositiveLimitConnector [AttrID=4]
         * @return
         */
        void setPositiveLimitConnector(eipScanner::cip::CipSint positiveLimitConnector);

        /**
         * @brief Sets NegativeLimitConnector [AttrID=5]
         * @return
         */
        void setNegativeLimitConnector(eipScanner::cip::CipSint negativeLimitConnector);

        /**
         * @brief Sets HomeSensorConnector [AttrID=6]
         * @return
         */
        void setHomeSensorConnector(eipScanner::cip::CipSint homeSensorConnector);

        /**
         * @brief Sets BrakeOutputConnector [AttrID=7]
         * @return
         */
        void setBrakeOutputConnector(eipScanner::cip::CipSint brakeOutputConnector);

        /**
         * @brief Sets TriggerPositionCaptureConnector [AttrID=8]
         * @return
         */
        void setTriggerPositionCaptureConnector(eipScanner::cip::CipSint triggerPositionCaptureConnector);

        /**
         * @brief Sets MaxDecelerationRate [AttrID=9]
         * @return
         */
        void setMaxDecelerationRate(eipScanner::cip::CipDint maxDecelerationRate);

        /**
         * @brief Sets StopSensorConnector [AttrID=10]
         * @return
         */
        void setStopSensorConnector(eipScanner::cip::CipSint stopSensorConnector);

        /**
         * @brief Sets FollowEncoderMode [AttrID=11]
         * @return
         */
        void setFollowEncoderMode(eipScanner::cip::CipSint followEncoderMode);

        /**
         * @brief Sets FollowDivisor [AttrID=12]
         * @return
         */
        void setFollowDivisor(eipScanner::cip::CipDint followDivisor);

        /**
         * @brief Sets FollowMultiplier [AttrID=13]
         * @return
         */
        void setFollowMultiplier(eipScanner::cip::CipDint followMultiplier);

        enum ConfigFlag
        {
            HomingEnable = 0,
            HomeSensorActiveLevel = 1,
            EnableInversion = 2,
            HLFBInversion = 3, // NOTE: The default if HIGH
            PositionCaptureActiveLevel = 4,
            SoftwareLimitEnable = 5
            // 6 - 31 are reserved
        };

        /**
         * @brief Sets a motor config flag
         */
        void setConfigFlag(ConfigFlag flag, bool value)
        {
            if (value)
            {
                _configRegister |= (1 << flag);
            }
            else
            {
                _configRegister &= ~(1 << flag);
            }
        }

    private:
        virtual std::vector<ReflexiveFieldReference> _getAttributeReferences();

        eipScanner::cip::CipDword _configRegister;
        eipScanner::cip::CipDint _softLimitPosition1;
        eipScanner::cip::CipDint _softLimitPosition2;
        eipScanner::cip::CipSint _positiveLimitConnector;
        eipScanner::cip::CipSint _negativeLimitConnector;
        eipScanner::cip::CipSint _homeSensorConnector;
        eipScanner::cip::CipSint _brakeOutputConnector;
        eipScanner::cip::CipSint _triggerPositionCaptureConnector;
        eipScanner::cip::CipDint _maxDecelerationRate;
        eipScanner::cip::CipSint _stopSensorConnector;
        eipScanner::cip::CipSint _followEncoderMode;
        eipScanner::cip::CipDint _followDivisor;
        eipScanner::cip::CipDint _followMultiplier;
    };
}
}
}
}


#endif // EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTORCONFIGOBJECT_H
