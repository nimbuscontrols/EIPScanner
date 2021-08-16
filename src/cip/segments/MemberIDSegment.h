//
// Created by Johannes Kauffmann on 08/09/21.
//

#ifndef EIPSCANNER_CIP_SEGMENTS_MEMBERIDSEGMENT_H
#define EIPSCANNER_CIP_SEGMENTS_MEMBERIDSEGMENT_H

#include <vector>

#include "LogicalSegment.h"

namespace eipScanner {
namespace cip {
namespace segments {

    class MemberIDSegment final : public LogicalSegment {
    public:

        /**
         * @brief Constructs a Logical Segment of Logical Type MemberID and encodes
         * the given value with header and format information
         * @param memberId The logical value of the Member ID
         * @param use_8_bit_segment True if the segment format should be 8bit
         * format, false if format should be 16bit format
         */
        MemberIDSegment(CipUint memberId, bool use_8_bit_segment=false);

        /**
         * @brief Gets the total encoded segment data
         * @return The encoded segment data
         */
        std::vector<uint8_t> data() const override;

        /**
         * @brief Gets the total size of the encoded segment in bytes
         * bytes
         * @return The size of the segment in bytes
         */
        uint8_t size() const override;

        /**
         * @brief Calculates the segment header byte for this Logical MemberID,
         * given its Logical Format
         * @return The segment header for this Logical MemberID
         */
        uint8_t getSegmentHeader() const override;

        /**
         * @brief Constructs a string representation of the Logical MemberID value
         * in the format "0xVALU" with the value in hexadecimal format
         * @return String with the MemberID as a hexadecimal value
         */
        std::string toString() const override;
    };

}
}
}

#endif // EIPSCANNER_CIP_SEGMENTS_MEMBERIDSEGMENT_H
