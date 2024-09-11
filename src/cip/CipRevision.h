//
// Created by Aleksey Timin on 12/8/19.
//

#ifndef EIPSCANNER_CIP_CIPREVISION_H
#define EIPSCANNER_CIP_CIPREVISION_H

#include <string>
#include "src/Types.h"

namespace eipScanner {
namespace cip {

	class CipRevision {
	public:
		CipRevision();
		CipRevision(CipUsint majorRevision, CipUsint minorRevision);

		bool operator==(const CipRevision& other) const;
		std::string toString() const;

		CipUsint getMajorRevision() const;
		CipUsint getMinorRevision() const;

	private:
		CipUsint _majorRevision;
		CipUsint _minorRevision;
	};
}
}

#endif  // EIPSCANNER_CIPREVISION_H
