//
// Created by Johannes Kauffmann on 08/09/21.
//

#ifndef EIPSCANNER_CIP_SEGMENTS_LOGICALSEGMENT_H
#define EIPSCANNER_CIP_SEGMENTS_LOGICALSEGMENT_H

#include <vector>

#include "ISegment.h"

namespace eipScanner {
namespace cip {
namespace segments {

    class LogicalSegment : public ISegment {
    protected:

        /**
         * @brief The LogicalType enum indicates the possible logical types which
         * encompass the most significant three bits of the segment format bits
         */
        enum class LogicalType : CipUsint {
            CLASS_ID         = 0x00,
            INSTANCE_ID      = 0x04,
            MEMBER_ID        = 0x08,
            CONNECTION_POINT = 0x0C,
            ATTRIBUTE_ID     = 0x10,
            SPECIAL          = 0x14,
            SERVICE_ID       = 0x18,
            RESERVED         = 0x1C
        };

        /**
         * @brief The LogicalFormat enum indicates the possible logical formats
         * which encompass the two least significant bits of the segment format
         * bits
         * @note Not all logical segments use these formats. The 32bit logical
         * format is only allowed for the Instance ID and Connection Point types
         */
        enum class LogicalFormat : CipUsint {
            FORMAT_8_BIT    = 0x00,
            FORMAT_16_BIT   = 0x01,
            FORMAT_32_BIT   = 0x02
        };

    protected:
        LogicalSegment(const std::vector<uint8_t> &value, LogicalFormat format);

    protected:
        LogicalFormat _format;
    };

}
}
}

#endif // EIPSCANNER_CIP_SEGMENTS_LOGICALSEGMENT_H
