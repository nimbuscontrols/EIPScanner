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
         * @brief Gets the total encoded segment data
         * @return The encoded segment data
         */
        virtual std::vector<uint8_t> data() const = 0;

        /**
         * @brief Gets the total size of the encoded segment in bytes
         * @return The size of the segment in bytes
         */
        virtual uint8_t size() const = 0;

        /**
         * @brief Calculates the segment header byte for the specific segment
         * @return The segment header byte
         */
        virtual uint8_t getSegmentHeader() const = 0;

        /**
         * @brief Constructs a string representation of the data value of this
         * segment in a human-readable form
         * @return The string representation of this segment
         */
        virtual std::string toString() const = 0;

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

        /**
         * @brief The constructor of a concrete segment should take in any
         * segment specific data and encode it accordingly
         * @param data The segment-specific encoded data
         */
        ISegment(const std::vector<uint8_t> &data);

    protected:

        /**
         * @brief This member variable contains the encoded segment data
         * (including header and padding details) at all times
         */
        std::vector<uint8_t> _data;
    };

}
}
}

#endif // EIPSCANNER_CIP_SEGMENTS_ISEGMENT_H
