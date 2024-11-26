//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_EPATH_H
#define EIPSCANNER_CIP_EPATH_H

#include <cstdint>
#include <string>
#include <vector>

#include "Types.h"
#include "segments/ISegment.h"

namespace eipScanner {
namespace cip {
	class EPath {
	public:
		EPath();
		explicit EPath(CipUint classId);
		EPath(CipUint classId, CipUint objectId);
		EPath(CipUint classId, CipUint objectId, CipUint attributeId);

        /**
         * @brief Constructs an EPath based on a collection of segments
         * @param segments Ordered collection of segments which the EPath should
         * consist of
         */
        EPath(const std::vector<segments::ISegment::SPtr> &segments);

        /**
         * @brief packPaddedPath Either encodes the class/object/attribute id
         * values to the corresponding logical segments, or asks all segments in
         * the segment collection to encode themselves
         * @param use_8_bit_path_segments True if using 8bit logical format to
         * encode the data, false if using 16bit logical format. Only has effect
         * on the class/object/attribute id the EPath was constructed with
         * @return Encoded EPath as contiguous sequence of bytes
         */
		std::vector<uint8_t> packPaddedPath(bool use_8_bit_path_segments=false) const;

        /**
         * @brief expandPaddedPath If the EPath was default-constructed, this
         * functions allows to fill the EPath with class/object/attribute id
         * values, encoded in a user-specified format. However, if the EPath was
         * constructed with a collection of segments, this function does nothing
         * @param data Class/object/attribute id logical segments encoded in a
         * user-specified format
         */
		void expandPaddedPath(const std::vector<uint8_t>& data);

		CipUint getClassId() const;
		CipUint getObjectId() const;
		CipUint getAttributeId() const;

        /**
         * @brief Calculates the total size of the encoded EPath in 16bit words.
         * Depending on how the EPath was constructed, either calculates the size
         * of the class/object/attribute id logical segments, or asks all segments
         * in the collection of segments to measure themselves
         * @param use_8_bit_path_segments True if the class/object/attribute id
         * logical segments should be counted with an 8bit logical format, false
         * if these should be counted as 16bit logical format. Has no effect when
         * the EPath was constructed with a collection of segments
         * @return The total size of the encoded EPath in 16bit words
         */
		CipUsint getSizeInWords(bool use_8_bit_path_segments=false) const;

        /**
         * @brief Constructs a string representation of the EPath. Depending on
         * whether or not the EPath was constructed with a collection of segments
         * or a class/object/attribute id, it either constructs a string of the
         * EPath with class/object/attribute id values or a representation of
         * every segment
         * @return String representation of the EPath with class/object/attribute
         * id values or string representations of the segments
         */
		std::string toString() const;

        /**
         * @brief Compares the EPath for equality to the other EPath, however,
         * it only checks for the class/object/attribute id values. In case of an
         * EPath with a segment collection, always returns true
         * @param other The EPath to compare this EPath with
         * @return True if EPath was constructed with segment collection, independent
         * of the equality of those segments OR the class/object/attribute id
         * values all match, false if these don't
         */
		bool operator==(const EPath& other) const;

	private:
		CipUint _classId;
		CipUint _objectId;
		CipUint _attributeId;
		CipUsint _size;
        std::vector<segments::ISegment::SPtr> _segments;
	};
}
}
#endif  // EIPSCANNER_CIP_EPATH_H
