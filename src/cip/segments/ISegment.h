//
// Created by Johannes Kauffmann on 08/06/21.
//

#ifndef EIPSCANNER_CIP_SEGMENTS_ISEGMENT_H
#define EIPSCANNER_CIP_SEGMENTS_ISEGMENT_H

#include <cstdint>
#include <memory>
#include <vector>

#include "cip/Types.h"

namespace eipScanner {
namespace cip {
namespace segments {

    class ISegment {
    public:
        using SPtr = std::shared_ptr<ISegment>;

        /**
         * @brief Encodes the segment data
         * @return The encoded segment data
         */
        virtual std::vector<uint8_t> encode() const = 0;

        /**
         * @brief Calculates the size of the segment in bytes
         * @return The size of the segment in bytes
         */
        virtual uint8_t getSize() const = 0;

    protected:

        /**
         * @brief The SegmentType enum encompasses the three most significant bits
         * of the segment header
         */
        enum class SegmentType : CipUsint {
            PORT_SEGMENT            = 0x00,
            LOGICAL_SEGMENT         = 0x20,
            NETWORK_SEGMENT         = 0x40,
            SYMBOLIC_SEGMENT        = 0x60,
            DATA_SEGMENT            = 0x80,
            DATA_TYPE_CONSTRUCTED   = 0xA0,
            DATA_TYPE_ELEMENTARY    = 0xC0,
            RESERVED                = 0xE0
        };

    protected:
        ISegment(const std::vector<uint8_t> &data);

    protected:
        std::vector<uint8_t> _data;
    };

}
}
}

#endif // EIPSCANNER_CIP_SEGMENTS_ISEGMENT_H
