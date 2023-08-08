//
// Created by Johannes Kauffmann on 08/06/21.
//

#include "ISegment.h"

namespace eipScanner {
namespace cip {
namespace segments {

    ISegment::ISegment(const std::vector<uint8_t> &data) :
        _data(data) {
    }

}
}
}
