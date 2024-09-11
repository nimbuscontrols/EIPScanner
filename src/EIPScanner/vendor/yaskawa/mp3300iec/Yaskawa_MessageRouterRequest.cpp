#include "EIPScanner/utils/Buffer.h"
#include "EIPScanner/vendor/yaskawa/mp3300iec/Yaskawa_MessageRouterRequest.h"
#include "EIPScanner/vendor/yaskawa/mp3300iec/Yaskawa_EPath.h"

namespace eipScanner {
namespace cip {
	using utils::Buffer;

	Yaskawa_MessageRouterRequest::Yaskawa_MessageRouterRequest(CipUsint serviceCode,
			const Yaskawa_EPath& ePath, const std::vector<uint8_t> data)
			: _serviceCode{serviceCode}
			, _ePath{ePath}
			, _data(data) {
	}

	Yaskawa_MessageRouterRequest::~Yaskawa_MessageRouterRequest() = default;

	std::vector<uint8_t> Yaskawa_MessageRouterRequest::pack() const {
		Buffer buffer;
		buffer << _serviceCode
			<< _ePath.getSizeInWords()
			<< _ePath.packPaddedPath()
			<< _data;

		return buffer.data();
	}
}
}
