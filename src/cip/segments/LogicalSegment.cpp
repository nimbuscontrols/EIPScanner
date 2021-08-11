//
// Created by Johannes Kauffmann on 08/09/21.
//

#include "LogicalSegment.h"

namespace eipScanner {
namespace cip {
namespace segments {

    LogicalSegment::LogicalSegment(const std::vector<uint8_t> &value, LogicalFormat format)
        : ISegment  (value)
        ,_format    (format) {

    }

}
}
}
