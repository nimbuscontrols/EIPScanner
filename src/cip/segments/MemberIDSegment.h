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
         * @brief Constructs a Logical Segment of Logical Type MemberID
         * @param memberId The logical value of the Member ID
         * @param use_8_bit_segment True if the segment format should be 8bit
         * format, false if format should be 16bit format
         */
        MemberIDSegment(CipUint memberId, bool use_8_bit_segment=false);

        /**
         * @brief Encodes the MemberID Logical Segment with the correct segment
         * header, possible padding and little endian encoded data
         * @return Vector of bytes with the complete segment encoded
         */
        std::vector<uint8_t> encode() const override;

        /**
         * @brief Calculates the size of the complete MemberID Logical Segment in
         * bytes
         * @return The size of the complete segment in bytes
         */
        uint8_t getSize() const override;
    };

}
}
}

#endif // EIPSCANNER_CIP_SEGMENTS_MEMBERIDSEGMENT_H
