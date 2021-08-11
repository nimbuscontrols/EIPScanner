//
// Created by Johannes Kauffmann on 08/09/21.
//

#include "MemberIDSegment.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace segments {

    using utils::Buffer;

    MemberIDSegment::MemberIDSegment(CipUint memberId, bool use_8_bits)
        : LogicalSegment({}, LogicalFormat::FORMAT_16_BIT)
    {
        if (use_8_bits) {
            _format = LogicalFormat::FORMAT_16_BIT;
        }

        // Convert MemberID to uint8_t or two bytes of uint16_t
        Buffer buffer;

        if (_format == LogicalFormat::FORMAT_8_BIT) {
            buffer << static_cast<CipUsint>(memberId);
        } else {
            buffer << memberId;
        }

        _data = buffer.data();
    }

    std::vector<uint8_t> MemberIDSegment::encode() const
    {
        Buffer buffer;

        // The segment header consists of the the segment type, logical type and logical format
        CipUsint header = static_cast<CipUsint>(SegmentType::LOGICAL_SEGMENT)
                        | static_cast<CipUsint>(LogicalType::MEMBER_ID)
                        | static_cast<CipUsint>(_format);

        buffer << header;

        // Check for padding
        if (_format == LogicalFormat::FORMAT_16_BIT) {
            buffer << static_cast<CipUsint>(0x00);
        }

        buffer << _data;

        return buffer.data();
    }

    uint8_t MemberIDSegment::getSize() const
    {
        uint8_t size = _data.size() + 1;

        // Check for padding
        if (_format == LogicalFormat::FORMAT_16_BIT) {
            size++;
        }

        return size;
    }

}
}
}
