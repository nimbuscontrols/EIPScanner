//
// Created by Johannes Kauffmann on 08/06/21.
//

#include "DataSegment.h"

namespace eipScanner {
namespace cip {
namespace segments {

    DataSegment::DataSegment(const std::vector<uint8_t> &data)
        : ISegment(data) {
    }

}
}
}
