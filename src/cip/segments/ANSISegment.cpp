//
// Created by Johannes Kauffmann on 08/06/21.
//

#include <vector>

#include "ANSISegment.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace segments {

    using eipScanner::cip::segments::DataSegment;
    using eipScanner::utils::Buffer;

    ANSISegment::ANSISegment(const std::vector<uint8_t>& data)
        : DataSegment(data) {
    }

    std::vector<uint8_t> ANSISegment::encode() const
    {
        Buffer buffer;

        // Stitch header together from the segment type and segment subtype.
        CipUsint header = static_cast<CipUsint>(SegmentType::DATA_SEGMENT)
                        | static_cast<CipUsint>(SubType::ANSI_EXTENDED_SYMBOL_SEGMENT);

        // Add the header, symbol size and data.
        buffer << header << static_cast<CipUsint>(_data.size()) << _data;

        // Add optional padding if the size is of odd length.
        if (_data.size() % 2 != 0) {
            buffer << static_cast<CipUsint>(0x00);
        }

        return buffer.data();
    }

    uint8_t ANSISegment::getSize() const
    {
        // Size is amount of chars + header and symbol length.
        uint8_t size = _data.size() + 2;

        // Check for padding.
        if (size % 2 != 0) {
            size++;
        }

        return size;
    }

}
}
}
