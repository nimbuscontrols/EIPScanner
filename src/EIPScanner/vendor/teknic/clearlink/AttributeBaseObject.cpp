//
// Created by Jan Ritzenhoff on 8/29/2024.
//

#include "EIPScanner/vendor/teknic/clearlink/AttributeBaseObject.h"

#include <memory>

namespace eipScanner
{
namespace vendor
{
namespace teknic
{
namespace clearlink
{
    AttributeBaseObject::AttributeBaseObject(eipScanner::cip::CipUint classId, eipScanner::cip::CipUint instanceId,
                                             const eipScanner::SessionInfoIf::SPtr sessionInfo, const eipScanner::MessageRouter::SPtr messageRouter)
        : BaseObject(classId, instanceId), _sessionInfo(sessionInfo), _messageRouter(messageRouter) {}

    void AttributeBaseObject::_getAllAttributesIndividually()
    {
        std::vector<ReflexiveFieldReference> classAttributes = _getAttributeReferences();

        for (uint8_t fieldIndex = 0; fieldIndex < classAttributes.size(); ++fieldIndex)
        {

            // actually send the explicit message
            auto getAttributeResponse = _messageRouter->sendRequest(_sessionInfo, eipScanner::cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
                                                                    eipScanner::cip::EPath(getClassId(), getInstanceId(), fieldIndex + 1));

            // check the getAttributeResponse code
            if (getAttributeResponse.getGeneralStatusCode() != eipScanner::cip::GeneralStatusCodes::SUCCESS)
            {
                logGeneralAndAdditionalStatus(getAttributeResponse);
                // throw std::runtime_error("Failed to read attribute with status code: " + std::to_string(getAttributeResponse.getGeneralStatusCode()));
                continue;
            }

            eipScanner::utils::Buffer getBuffer(getAttributeResponse.getData());

            auto fieldReference = classAttributes[fieldIndex];
            // pass a reference of the getBuffer to the lambda function and the first argument of the lambda function is the actual field reference
            std::visit([&getBuffer](auto &fieldRef)
                       { getBuffer >> fieldRef; },
                       fieldReference);
        }
    }

    void AttributeBaseObject::_setAllAttributesIndividually()
    {
        std::vector<ReflexiveFieldReference> classAttributes = _getAttributeReferences();

        for (uint8_t fieldIndex = 0; fieldIndex < classAttributes.size(); ++fieldIndex)
        {

            auto fieldReference = classAttributes[fieldIndex];

            eipScanner::utils::Buffer setBuffer;

            // pass a reference of the buffer to the lambda function
            std::visit([&setBuffer](auto &fieldRef)
                       { setBuffer << fieldRef; },
                       fieldReference);

            // actually send the explicit message
            auto setAttributeResponse = _messageRouter->sendRequest(_sessionInfo, eipScanner::cip::ServiceCodes::SET_ATTRIBUTE_SINGLE,
                                                                    eipScanner::cip::EPath(getClassId(), getInstanceId(), fieldIndex + 1),
                                                                    setBuffer.data());

            // check the setAttributeResponse code
            if (setAttributeResponse.getGeneralStatusCode() != eipScanner::cip::GeneralStatusCodes::SUCCESS)
            {
                logGeneralAndAdditionalStatus(setAttributeResponse);
                throw std::runtime_error("Failed to read attribute with status code: " + std::to_string(setAttributeResponse.getGeneralStatusCode()));
            }
        }
    }

}
}
}
}