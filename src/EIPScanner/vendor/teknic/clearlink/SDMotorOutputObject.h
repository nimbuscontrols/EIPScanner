//
// Created by Jan Ritzenhoff on 8/29/2024
//

// Based on:
//	Step and Direction Motor Output Object (66hex - 4 instances)
//		* https://www.teknic.com/files/downloads/clearlink_ethernet-ip_object_reference.pdf#page=46

#ifndef EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTOROUTPUTOBJECT_H
#define EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTOROUTPUTOBJECT_H

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
     * @class SDMotorOutputObject
     *
     * @brief Implements interface to Identity Object (0x66).
     *
     * It reads all data from the CIP instance in the constructor
     */
    class SDMotorOutputObject : public AttributeBaseObject
    {
    public:
        static const eipScanner::cip::CipUint CLASS_ID = 0x66;

        /**
         * @brief Creates an instance and reads all its data via EIP
         * @note Used for testing
         * @param instanceId
         * @param si
         * @param messageRouter
         */
        SDMotorOutputObject(eipScanner::cip::CipUint instanceId, const eipScanner::SessionInfoIf::SPtr si, const eipScanner::MessageRouter::SPtr messageRouter);

        // -------------------------------------------------------------

        void setAllAttributesIndividually();

        // -------------------------------------------------------------

        /**
         * @brief Gets MoveDistance [AttrID=1]
         * @return
         */
        eipScanner::cip::CipDint getMoveDistance() const;

        /**
         * @brief Gets JogVelocity [AttrID=2]
         * @return
         */
        eipScanner::cip::CipDint getJogVelocity() const;

        /**
         * @brief Gets VelocityLimitForPositionalMove [AttrID=3]
         * @return
         */
        eipScanner::cip::CipUdint getVelocityLimitForPositionalMove() const;

        /**
         * @brief Gets AccelerationLimitForPositionalMove [AttrID=4]
         * @return
         */
        eipScanner::cip::CipUdint getAccelerationLimitForPositionalMove() const;

        /**
         * @brief Gets DecelerationLimit [AttrID=5]
         * @return
         */
        eipScanner::cip::CipUdint getDecelerationLimit() const;

        /**
         * @brief Gets OutputRegister [AttrID=6]
         * @return
         */
        eipScanner::cip::CipDword getOutputRegister() const;

        /**
         * @brief Gets AddToPosition [AttrID=7]
         * @return
         */
        eipScanner::cip::CipDint getAddToPosition() const;

        // -------------------------------------------------------------

        /**
         * @brief Sets MoveDistance [AttrID=1]
         * @return
         */
        void setMoveDistance(eipScanner::cip::CipDint moveDistance);

        /**
         * @brief Sets JogVelocity [AttrID=2]
         * @return
         */
        void setJogVelocity(eipScanner::cip::CipDint jogVelocity);

        /**
         * @brief Sets VelocityLimitForPositionalMove [AttrID=3]
         * @return
         */
        void setVelocityLimitForPositionalMove(eipScanner::cip::CipUdint velocityLimitForPositionalMove);

        /**
         * @brief Sets AccelerationLimitForPositionalMove [AttrID=4]
         * @return
         */
        void setAccelerationLimitForPositionalMove(eipScanner::cip::CipUdint accelerationLimitForPositionalMove);

        /**
         * @brief Sets DecelerationLimit [AttrID=5]
         * @return
         */
        void setDecelerationLimit(eipScanner::cip::CipUdint decelerationLimit);

        /**
         * @brief Sets OutputRegister [AttrID=6]
         * @return
         */
        void setOutputRegister(eipScanner::cip::CipDword outputRegister);

        /**
         * @brief Sets AddToPosition [AttrID=7]
         * @return
         */
        void setAddToPosition(eipScanner::cip::CipDint addToPosition);

        enum OutputFlag
        {
            Enable = 0,
            AbsoluteMove = 1,
            HomingMove = 2,
            LoadPositionMove = 3,
            LoadVelocityMove = 4,
            SoftwareEStop = 5,
            ClearAlerts = 6,
            ClearMotorFault = 7,
            // 8-31 are reserved
        };

        /**
         * @brief Sets a motor flag
         */
        void setOutputFlag(OutputFlag flag, bool value)
        {
            if (value)
            {
                _outputRegister |= (1 << flag);
            }
            else
            {
                _outputRegister &= ~(1 << flag);
            }
        }

        /**
         * @brief Checks if the motor is controlled to have a specific state
         */
        bool hasFlagSet(OutputFlag flag)
        {
            return _outputRegister & (1 << flag);
        }

        void repopulateFromAdapter()
        {
            _getAllAttributesIndividually();
        }

    private:
        virtual std::vector<ReflexiveFieldReference> _getAttributeReferences();

        eipScanner::cip::CipDint _moveDistance;
        eipScanner::cip::CipDint _jogVelocity;
        eipScanner::cip::CipUdint _velocityLimitForPositionalMove;
        eipScanner::cip::CipUdint _accelerationLimitForPositionalMove;
        eipScanner::cip::CipUdint _decelerationLimit;
        eipScanner::cip::CipDword _outputRegister;
        eipScanner::cip::CipDint _addToPosition;
    };
}
}
}
}

#endif // EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_SDMOTOROUTPUTOBJECT_H
