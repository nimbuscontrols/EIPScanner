//
// Created by Aleksey Timin on 11/16/19.
//

#include <utils/Buffer.h>
#include "MessageRouterRequest.h"
#include "EPath.h"

namespace eipScanner {
namespace cip {
	using utils::Buffer;

	MessageRouterRequest::MessageRouterRequest(ServiceCodes serviceCode,
			const EPath& ePath, const std::vector<uint8_t> data)
			: _serviceCode{serviceCode}
			, _ePath{ePath}
			, _data(data) {
	}

	MessageRouterRequest::~MessageRouterRequest() = default;

	std::vector<uint8_t> MessageRouterRequest::pack() const {
		Buffer buffer;
		buffer << static_cast<CipUsint >(_serviceCode)
			<< _ePath.packPaddedPath()
			<< _data;

		return buffer.data();
	}
}
}