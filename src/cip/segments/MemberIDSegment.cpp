//
// Created by Johannes Kauffmann on 08/09/21.
//

#include <iomanip>
#include <iostream>

#include "MemberIDSegment.h"
#include "utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace segments {

    using utils::Buffer;

    MemberIDSegment::MemberIDSegment(CipUint memberId, bool use_8_bits)
        : LogicalSegment({}, LogicalFormat::FORMAT_16_BIT)
    {
        // Store the correct format
        if (use_8_bits) {
            _format = LogicalFormat::FORMAT_8_BIT;
        }

        Buffer buffer;

        // The segment header consists of the the segment type, logical type and logical format
        CipUsint header = getSegmentHeader();

        buffer << header;

        // Convert MemberID to uint8_t or two bytes of uint16_t
        if (_format == LogicalFormat::FORMAT_8_BIT) {
            buffer << static_cast<CipUsint>(memberId);
        } else {
            // Add padding between header and data
            buffer << static_cast<CipUsint>(0x00);
            buffer << memberId;
        }

        _data = buffer.data();
    }

    std::vector<uint8_t> MemberIDSegment::data() const
    {
        return _data;
    }

    uint8_t MemberIDSegment::size() const
    {
        return _data.size();
    }

    uint8_t MemberIDSegment::getSegmentHeader() const
    {
        return static_cast<CipUsint>(SegmentType::LOGICAL_SEGMENT)
             | static_cast<CipUsint>(LogicalType::MEMBER_ID)
             | static_cast<CipUsint>(_format);
    }

    std::string MemberIDSegment::toString() const
    {
        // Format the hexadecimal value with 2 leading zero's
        std::stringstream stream;
        stream << "0x" << std::hex << std::setfill('0') << std::setw(2);

        int headerSize = 1;
        if (_format == LogicalFormat::FORMAT_16_BIT) {
            headerSize++;
        }

        // Create a buffer containing just the data, without the header
        Buffer buffer({_data.begin() + headerSize, _data.end()});

        // Decode to either 8bit or 16bit
        if (_format == LogicalFormat::FORMAT_8_BIT) {
            CipUsint value;
            buffer >> value;
            stream << int(value);
        } else {
            CipUint value;
            buffer >> value;
            stream << int(value);
        }

        return stream.str();
    }

}
}
}
