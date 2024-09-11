//
// Created by Aleksey Timin on 12/8/19.
//

#include "src/cip/CipRevision.h"

namespace eipScanner {
namespace cip {

	CipRevision::CipRevision()
		: _majorRevision(0)
		, _minorRevision(0) {
	}

	CipRevision::CipRevision(CipUsint majorRevision, CipUsint minorRevision)
		: _majorRevision(majorRevision)
		, _minorRevision(minorRevision) {
	}

	bool CipRevision::operator==(const CipRevision &other) const {
		return _majorRevision == other._majorRevision
			&& _minorRevision == other._minorRevision;
	}

	std::string CipRevision::toString() const {
		return std::to_string(_majorRevision) + "." + std::to_string(_minorRevision);
	}

	CipUsint CipRevision::getMajorRevision() const {
		return _majorRevision;
	}

	CipUsint CipRevision::getMinorRevision() const {
		return _minorRevision;
	}
}
}