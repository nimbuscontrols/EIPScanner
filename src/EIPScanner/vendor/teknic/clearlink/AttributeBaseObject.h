//
// Created by Jan Ritzenhoff on 8/29/2024.
//

#ifndef EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_ATTRIBUTEBASEOBJECT_H
#define EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_ATTRIBUTEBASEOBJECT_H

#include <variant>
#include <functional>
#include <vector>

#include "EIPScanner/cip/Types.h"
#include "EIPScanner/BaseObject.h"
#include "EIPScanner/SessionInfo.h"
#include "EIPScanner/MessageRouter.h"

#include "EIPScanner/utils/Buffer.h"

namespace eipScanner
{
namespace vendor
{
namespace teknic
{
namespace clearlink
{

    /**
     * @class AttributeBaseObject
     *
     * @brief Base class for all the CIP Objects that access all attributes independently using explicit messaging
     */
    class AttributeBaseObject : public eipScanner::BaseObject
    {
    public:
        /**
         * @brief Creates an CIP instance
         * @param classId the class ID of the the new instance
         * @param instanceId the instance ID of the the new instanc
         */
        AttributeBaseObject(eipScanner::cip::CipUint classId, eipScanner::cip::CipUint instanceId, const eipScanner::SessionInfoIf::SPtr sessionInfo, const eipScanner::MessageRouter::SPtr messageRouter);

    protected:
        using ReflexiveFieldReference = std::variant<std::reference_wrapper<eipScanner::cip::CipOctet>,
                                                     std::reference_wrapper<eipScanner::cip::CipBool>,
                                                     std::reference_wrapper<eipScanner::cip::CipByte>,
                                                     std::reference_wrapper<eipScanner::cip::CipWord>,
                                                     std::reference_wrapper<eipScanner::cip::CipDword>,
                                                     std::reference_wrapper<eipScanner::cip::CipUsint>,
                                                     std::reference_wrapper<eipScanner::cip::CipUint>,
                                                     std::reference_wrapper<eipScanner::cip::CipSint>,
                                                     std::reference_wrapper<eipScanner::cip::CipInt>,
                                                     std::reference_wrapper<eipScanner::cip::CipDint>,
                                                     std::reference_wrapper<eipScanner::cip::CipReal>,
                                                     std::reference_wrapper<eipScanner::cip::CipLreal>,
                                                     std::reference_wrapper<eipScanner::cip::CipLword>,
                                                     std::reference_wrapper<eipScanner::cip::CipLint>,
                                                     std::reference_wrapper<eipScanner::cip::CipUlint>>;
        // std::reference_wrapper<eipScanner::cip::CipUdint>, (this is typedeffed' to the same thing as the DWord. Including both in the variant confuses the compiler)

        virtual std::vector<ReflexiveFieldReference> _getAttributeReferences() = 0;

        /**
         * @brief Gets the whole assembly to the device as individual get_attribute_single messages
         */
        void _getAllAttributesIndividually();

        /**
         * @brief Sets the whole assembly to the device as individual set_attribute_single messages
         */
        void _setAllAttributesIndividually();

    private:
        const eipScanner::SessionInfoIf::SPtr _sessionInfo;
        const eipScanner::MessageRouter::SPtr _messageRouter;
    };
}
}
}
}

#endif // EIPSCANNER_VENDOR_TEKNIC_CLEARLINK_ATTRIBUTEBASEOBJECT_H
