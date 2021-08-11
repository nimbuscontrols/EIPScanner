//
// Created by Johannes Kauffmann on 08/06/21.
//

#ifndef EIPSCANNER_CIP_SEGMENTS_DATASEGMENT_H
#define EIPSCANNER_CIP_SEGMENTS_DATASEGMENT_H

#include <vector>

#include "cip/Types.h"
#include "ISegment.h"

namespace eipScanner {
namespace cip {
namespace segments {

    class DataSegment : public ISegment {
    protected:

        /**
         * @brief The SubType enum indicates the possible data segment subtypes
         * which encompass the 5 lower bits of the segment header
         */
        enum class SubType : CipUsint {
            SIMPLE_DATA_SEGMENT             = 0x00,
            ANSI_EXTENDED_SYMBOL_SEGMENT    = 0x11
        };

    protected:
        DataSegment(const std::vector<uint8_t> &data);
    };

}
}
}

#endif // EIPSCANNER_CIP_SEGMENTS_DATASEGMENT_H
