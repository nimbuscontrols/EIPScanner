//
// Created by flipback on 11/16/19.
//

#include "Buffer.h"

namespace eipScanner {
namespace utils {

	Buffer::Buffer(size_t capacity) : buffer(0), _position{0} {
		buffer.reserve(capacity);
	}

	Buffer::Buffer(const std::vector<uint8_t>& data) : buffer(data), _position{0} {

	}

	Buffer::Buffer() : buffer(0), _position{0} {
	}

	Buffer &Buffer::operator<<(uint8_t val) {
		buffer.push_back(val);
		return *this;
		;
	}

	Buffer &Buffer::operator>>(uint8_t &val) {
		val = buffer[_position++];
		return *this;

	}

	Buffer &Buffer::operator<<(int8_t val) {
		buffer.push_back(static_cast<uint8_t>(val));
		return *this;

	}

	Buffer &Buffer::operator>>(int8_t &val) {
		val = static_cast<uint8_t>(buffer[_position++]);
		return *this;
	}

	Buffer &Buffer::operator<<(uint16_t val) {
		buffer.push_back(static_cast<uint8_t>(val));
		buffer.push_back(static_cast<uint8_t>(val >> 8));
		return *this;
	}

	Buffer &Buffer::operator>>(uint16_t &val) {
		val = static_cast<uint16_t>(buffer[_position++]);
		val += static_cast<uint16_t>(buffer[_position++] << 8);
		return *this;
	}

	Buffer &Buffer::operator<<(int16_t val) {
		return *this << static_cast<uint16_t >(val);;
	}

	Buffer &Buffer::operator>>(int16_t &val) {
		return *this >> reinterpret_cast<uint16_t&>(val);
	}

	Buffer &Buffer::operator<<(uint32_t val) {
		buffer.push_back(static_cast<uint8_t>(val));
		buffer.push_back(static_cast<uint8_t>(val >> 8));
		buffer.push_back(static_cast<uint8_t>(val >> 16));
		buffer.push_back(static_cast<uint8_t>(val >> 24));

		return *this;
	}

	Buffer &Buffer::operator>>(uint32_t &val) {
		val = static_cast<uint32_t>(buffer[_position++]);
		val += static_cast<uint32_t>(buffer[_position++] << 8);
		val += static_cast<uint32_t>(buffer[_position++] << 16);
		val += static_cast<uint32_t>(buffer[_position++] << 24);

		return *this;
	}

	Buffer &Buffer::operator<<(int32_t val) {
		return *this << static_cast<uint32_t >(val);;
	}

	Buffer &Buffer::operator>>(int32_t &val) {
		return *this >> reinterpret_cast<uint32_t&>(val);
	}

	Buffer &Buffer::operator<<(float val) {
		return *this << static_cast<uint16_t>(val);
	}

	Buffer &Buffer::operator>>(float &val)  {
		return *this >> reinterpret_cast<uint16_t&>(val);
	}

	Buffer &Buffer::operator<<(double val) {
		return *this << static_cast<uint32_t>(val);
	}

	Buffer &Buffer::operator>>(double &val) {
		return *this >> reinterpret_cast<uint32_t&>(val);
	}

	Buffer &Buffer::operator<<(const std::vector<uint8_t> &val) {
		buffer.insert(buffer.end(), val.begin(), val.end());
		return *this;
	}

	Buffer &Buffer::operator>>(std::vector<uint8_t> &val) {
		std::copy(buffer.begin() + _position, buffer.begin() + _position + val.size(), val.begin());
		_position += val.size();
		return *this;
	}

	Buffer &Buffer::operator<<(const std::vector<uint16_t> &val) {
		for(auto& v : val) {
			buffer.push_back(static_cast<uint8_t>(v));
			buffer.push_back(static_cast<uint8_t>(v >> 8));
		}

		return *this;
	}

	Buffer &Buffer::operator>>(std::vector<uint16_t> &val) {
		for (auto& v : val) {
			v = static_cast<int16_t>(buffer[_position++]);
			v += static_cast<int16_t>(_position++ << 8);
		}

		return *this;
	}
}
}
