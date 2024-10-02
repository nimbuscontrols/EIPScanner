//
// Created by Jan Ritzenhoff on 8/29/2024
//

// Based on:
//	Step and Direction Motor Input Object (65hex - 4 instances)
//		* https://www.teknic.com/files/downloads/clearlink_ethernet-ip_object_reference.pdf#page=41

#ifndef EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTORINPUTOBJECT_H
#define EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTORINPUTOBJECT_H

#include "EIPScanner/cip/CipRevision.h"
#include "EIPScanner/cip/CipString.h"

#include "EIPScanner/utils/Logger.h"

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
     * @class SDMotorInputObject
     *
     * @brief Implements interface to Identity Object (0x65).
     *
     * It reads all data from the CIP instance in the constructor
     */
    class SDMotorInputObject : public AttributeBaseObject
    {
    public:
        static const eipScanner::cip::CipUint CLASS_ID = 0x65;

        /**
         * @brief Creates an instance and reads all its data via EIP
         * @note Used for testing
         * @param instanceId
         * @param si
         * @param messageRouter
         */
        SDMotorInputObject(eipScanner::cip::CipUint instanceId, const eipScanner::SessionInfoIf::SPtr si, const eipScanner::MessageRouter::SPtr messageRouter);

        /**
         * @brief Gets CommandedPosition [AttrID=1]
         * @return
         */
        eipScanner::cip::CipDint getCommandedPosition() const;

        /**
         * @brief Gets CommandedVelocity [AttrID=2]
         * @return
         */
        eipScanner::cip::CipDint getCommandedVelocity() const;

        /**
         * @brief Gets TargetPosition [AttrID=3]
         * @return
         */
        eipScanner::cip::CipDint getTargetPosition() const;

        /**
         * @brief Gets TargetVelocity [AttrID=4]
         * @return
         */
        eipScanner::cip::CipDint getTargetVelocity() const;

        /**
         * @brief Gets CapturedPosition [AttrID=5]
         * @return
         */
        eipScanner::cip::CipDint getCapturedPosition() const;

        /**
         * @brief Gets MeasuredTorque [AttrID=6]
         * @return
         */
        eipScanner::cip::CipReal getMeasuredTorque() const;

        /**
         * @brief Gets MotorStatus [AttrID=7]
         * @return
         */
        eipScanner::cip::CipDword getMotorStatus() const;

        /**
         * @brief Gets MotorShutdowns [AttrID=8]
         * @return
         */
        eipScanner::cip::CipDword getMotorShutdowns() const;

        enum InputState
        {
            AtTargetPosition = 0,
            StepsActive = 1,
            AtVelocity = 2,
            MoveDirection = 3,
            InPositiveLimit = 4,
            InNegativeLimit = 5,
            InEStopSensor = 6,
            InHomeSensor = 7,
            Homing = 8,
            MotorInFault = 9,
            Enabled = 10,
            OutsideSoftLimits = 11,
            PositionalMove = 12,
            HasHomed = 13,
            HLFB_On = 14,
            HasTorqueMeasurement = 15,
            ReadyToHome = 16,
            ShutdownsPresent = 17,
            AddToPositionAck = 18,
            LoadPositionMoveAck = 19,
            LoadVelocityMoveAck = 20,
            ClearMotorFaultAck = 21,
            NUM_STATES = 22
            // 22 - 31 are reserved
        };

        /**
         * @brief Prints out all of the states that are true
         */
        void printMotorInputState(eipScanner::utils::Logger logger);

        /**
         * @brief Checks if the motor is in a specific state
         */
        bool isInState(InputState state)
        {
            return _motorStatus & (1 << state);
        }

        void repopulateFromAdapter()
        {
            _getAllAttributesIndividually();
        }

    private:
        virtual std::vector<ReflexiveFieldReference> _getAttributeReferences();

        eipScanner::cip::CipDint _commandedPosition;
        eipScanner::cip::CipDint _commandedVelocity;
        eipScanner::cip::CipDint _targetPosition;
        eipScanner::cip::CipDint _targetVelocity;
        eipScanner::cip::CipDint _capturedPosition;
        eipScanner::cip::CipReal _measuredTorque;
        eipScanner::cip::CipDword _motorStatus;
        eipScanner::cip::CipDword _motorShutdowns;

        std::string stateToString(InputState state)
        {
            switch (state)
            {
            case AtTargetPosition:
                return "AtTargetPosition";
            case StepsActive:
                return "StepsActive";
            case AtVelocity:
                return "AtVelocity";
            case MoveDirection:
                return "MoveDirection";
            case InPositiveLimit:
                return "InPositiveLimit";
            case InNegativeLimit:
                return "InNegativeLimit";
            case InEStopSensor:
                return "InEStopSensor";
            case InHomeSensor:
                return "InHomeSensor";
            case Homing:
                return "Homing";
            case MotorInFault:
                return "MotorInFault";
            case Enabled:
                return "Enabled";
            case OutsideSoftLimits:
                return "OutsideSoftLimits";
            case PositionalMove:
                return "PositionalMove";
            case HasHomed:
                return "HasHomed";
            case HLFB_On:
                return "HLFB_On";
            case HasTorqueMeasurement:
                return "HasTorqueMeasurement";
            case ReadyToHome:
                return "ReadyToHome";
            case ShutdownsPresent:
                return "ShutdownsPresent";
            case AddToPositionAck:
                return "AddToPositionAck";
            case LoadPositionMoveAck:
                return "LoadPositionMoveAck";
            case LoadVelocityMoveAck:
                return "LoadVelocityMoveAck";
            case ClearMotorFaultAck:
                return "ClearMotorFaultAck";
            case NUM_STATES:
                return "NUM_STATES";
            default:
                return "Unknown State";
            }
        }
    };
}
}
}
}

#endif // EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTORINPUTOBJECT_H
