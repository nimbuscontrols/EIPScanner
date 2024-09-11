//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_UTILS_BUFFER_H
#define EIPSCANNER_UTILS_BUFFER_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include "src/cip/CipString.h"
#include "src/cip/CipRevision.h"
#include "src/sockets/EndPoint.h"

namespace eipScanner {
namespace utils {
	/**
	 * @class Buffer
	 *
	 * @brief Implements decode and encode data according CIP specification
	 *
	 * An example:
	 * @code
	 * Buffer buffer1();
	 * cip::CipUint var1 = 1;
	 * cip::CipDint var2 = 0xaa00000;
	 *
	 * buffer1 << var1 << var2;
	 *
	 * buffer1.data(); # => {0x01, 0x0, 0x0 ,0x0, 0x0, 0xaa}
	 * @endcode
	 */
	class Buffer {
	public:

		/**
		 * Creates an empty buffer
		 * @param capacity the size that will be reserved in the buffer
		 */
		explicit Buffer(size_t capacity);

		/**
		 * Creates a buffer that contains the given data
		 * @param data The data to encode
		 */
		explicit Buffer(const std::vector<uint8_t>& data);

		/**
		 * Creates an empty buffer
		 */
		Buffer();

		Buffer& operator << (uint8_t val);
		Buffer& operator >> (uint8_t& val);

		Buffer& operator << (int8_t val);
		Buffer& operator >> (int8_t& val);

		Buffer& operator << (uint16_t val);
		Buffer& operator >> (uint16_t& val);

		Buffer& operator << (int16_t val);
		Buffer& operator >> (int16_t& val);

		Buffer& operator << (uint32_t val);
		Buffer& operator >> (uint32_t& val);

		Buffer& operator << (int32_t val);
		Buffer& operator >> (int32_t& val);

		Buffer& operator << (uint64_t val);
		Buffer& operator >> (uint64_t& val);

		Buffer& operator << (int64_t val);
		Buffer& operator >> (int64_t& val);

		Buffer& operator << (float val);
		Buffer& operator >> (float & val);

		Buffer& operator << (double val);
		Buffer& operator >> (double& val);

		Buffer& operator << (const std::vector<uint8_t>& val);
		Buffer& operator >> (std::vector<uint8_t>& val);

		Buffer& operator << (const std::vector<uint16_t>& val);
		Buffer& operator >> (std::vector<uint16_t>& val);

		template <typename T>
		utils::Buffer& operator<<(const cip::CipBaseString<T>& cipSting) {
			return *this << cipSting.getLength() << cipSting.getData();
		}

		template <typename T>
		utils::Buffer& operator>>(cip::CipBaseString<T>& cipSting) {
			T length = 0;
			*this >> length;
			std::vector<uint8_t> data(length);
			*this >> data;

			cipSting = cip::CipBaseString<T>(data);
			return *this;
		}

		Buffer& operator << (cip::CipRevision v);
		Buffer& operator >> (cip::CipRevision& val);

		Buffer& operator << (sockets::EndPoint v);
		Buffer& operator >> (sockets::EndPoint& val);

		std::vector<uint8_t> data() const { return _buffer; }
		size_t size() const { return _buffer.size(); }
		size_t pos() const { return _position; }
		bool isValid() const { return _position <= _buffer.size(); }
		bool empty() const  { return  _position >= _buffer.size(); }
	private:
		std::vector<uint8_t> _buffer;
		size_t _position;
	};
}
}
#endif  // EIPSCANNER_BUFFER_H
