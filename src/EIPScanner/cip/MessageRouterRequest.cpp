//
// Created by Aleksey Timin on 11/16/19.
//

#include "EIPScanner/utils/Buffer.h"

#include "EIPScanner/cip/MessageRouterRequest.h"
#include "EIPScanner/cip/EPath.h"

namespace eipScanner {
namespace cip {
	using utils::Buffer;

	MessageRouterRequest::MessageRouterRequest(CipUsint serviceCode,
			const EPath& ePath, const std::vector<uint8_t> data, bool use_8_bit_path_segments)
			: _serviceCode{serviceCode}
			, _ePath{ePath}
			, _data(data)
            , _use_8_bit_path_segments(use_8_bit_path_segments) {
	}

	MessageRouterRequest::~MessageRouterRequest() = default;

	std::vector<uint8_t> MessageRouterRequest::pack() const {
		Buffer buffer;
		buffer << _serviceCode
			<< _ePath.getSizeInWords(_use_8_bit_path_segments)
			<< _ePath.packPaddedPath(_use_8_bit_path_segments)
			<< _data;

		return buffer.data();
	}
}
}