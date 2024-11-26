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
         * @brief Constructs a CIP DataSegment of type ANSI Extended Symbol Segment
         * and encodes the data with header, symbol length, data and possible padding
         * @param data The segment data, excluding details such as length or padding
         */
        ANSISegment(const std::vector<uint8_t>& data);

        /**
         * @brief Gets the total encoded segment data
         * @return The encoded segment data
         */
        std::vector<uint8_t> data() const override;

        /**
         * @brief Gets the total size of the encoded segment in bytes
         * @return The size of the segment in bytes
         */
        uint8_t size() const override;

        /**
         * @brief Calculates the segment header byte for this ANSI Extended Symbol
         * Segment
         * @return The segment header for this ANSI Extended Symbol Segment
         */
        uint8_t getSegmentHeader() const override;

        /**
         * @brief Constructs a string representation of the data value of this
         * ANSI Extended Symbol Segment
         * @return The string representation of the data of this segment
         */
        std::string toString() const override;

    };

}
}
}

#endif // EIPSCANNER_CIP_SEGMENTS_ANSISEGMENT_H
