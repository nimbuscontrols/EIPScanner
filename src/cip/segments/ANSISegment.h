//
// Created by Johannes Kauffmann on 08/06/21.
//

#ifndef EIPSCANNER_CIP_SEGMENTS_ANSISEGMENT_H
#define EIPSCANNER_CIP_SEGMENTS_ANSISEGMENT_H

#include <memory>

#include "cip/CipString.h"
#include "DataSegment.h"

namespace eipScanner {
namespace cip {
namespace segments {

    class ANSISegment final : public DataSegment {
    public:

        /**
         * @brief Constructs a CIP DataSegment of type ANSI Extended Symbol Segment.
         * @param data The segment data, excluding details such as length or padding
         */
        ANSISegment(const std::vector<uint8_t>& data);

        /**
         * @brief Encodes the segment data with the header, symbol length and data
         * with possible padding included
         * @return The encoded segment data
         */
        std::vector<uint8_t> encode() const override;

        /**
         * @brief Calculates the size of the segment in bytes, including the
         * header, symbol length and possible padding
         * @return The size of the segment in bytes
         */
        uint8_t getSize() const override;
    };

}
}
}

#endif // EIPSCANNER_CIP_SEGMENTS_ANSISEGMENT_H
