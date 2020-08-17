#ifndef EIPSCANNER_YASKAWA_EPATH_H
#define EIPSCANNER_YASKAWA_EPATH_H

#include <vector>
#include <string>
#include <cstdint>

#include "cip/Types.h"


namespace eipScanner {
namespace cip {
	class Yaskawa_EPath {
	public:
		Yaskawa_EPath();
		explicit Yaskawa_EPath(CipUint classId);
		Yaskawa_EPath(CipUint classId, CipUint objectId);
		Yaskawa_EPath(CipUint classId, CipUint objectId, CipUint attributeId);
		std::vector<uint8_t> packPaddedPath() const;
		void expandPaddedPath(const std::vector<uint8_t>& data);

		CipUsint getClassId() const;
		CipUsint getObjectId() const;
		CipUsint getAttributeId() const;

		CipUsint getSizeInWords() const;

		std::string toString() const;
		bool operator==(const Yaskawa_EPath& other) const;

	private:
		CipUsint _classId;
		CipUsint _objectId;
		CipUsint _attributeId;

		CipUsint _size;
	};
}
}


#endif  // EIPSCANNER_YASKAWA_EPATH_H
