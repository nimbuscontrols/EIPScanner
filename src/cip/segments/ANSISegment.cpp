//
// Created by Johannes Kauffmann on 08/06/21.
//

#include <vector>

#include "ANSISegment.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace segments {

    using segments::DataSegment;
    using utils::Buffer;

    ANSISegment::ANSISegment(const std::vector<uint8_t>& data) : DataSegment({})
    {
        Buffer buffer;

        // Stitch header together from the segment type and segment subtype.
        CipUsint header = getSegmentHeader();

        // Add the header, symbol size and data.
        buffer << header << static_cast<CipUsint>(data.size()) << data;

        // Add optional padding if the size is of odd length.
        if (data.size() % 2 != 0) {
            buffer << static_cast<CipUsint>(0x00);
        }

        _data = buffer.data();
    }

    std::vector<uint8_t> ANSISegment::data() const
    {
        return _data;
    }

    uint8_t ANSISegment::size() const
    {
        return _data.size();
    }

    uint8_t ANSISegment::getSegmentHeader() const
    {
        return static_cast<CipUsint>(SegmentType::DATA_SEGMENT)
             | static_cast<CipUsint>(SubType::ANSI_EXTENDED_SYMBOL_SEGMENT);
    }

    std::string ANSISegment::toString() const
    {
        int paddingSize = 0;
        if (_data.back() == 0x00) {
            paddingSize++;
        }
        // Construct a string without the header and optional padding
        return std::string( _data.begin() + 2, _data.end() - paddingSize);
    }

}
}
}
