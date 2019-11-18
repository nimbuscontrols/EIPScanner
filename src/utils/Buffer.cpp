//
// Created by flipback on 11/16/19.
//

#include "Buffer.h"

namespace eipScanner {
namespace utils {

	Buffer::Buffer(size_t capacity) : _buffer(0), _position{0} {
		_buffer.reserve(capacity);
	}

	Buffer::Buffer(const std::vector<uint8_t>& data) : _buffer(data), _position{0} {

	}

	Buffer::Buffer() : _buffer(0), _position{0} {
	}

	Buffer &Buffer::operator<<(uint8_t val) {
		_buffer.push_back(val);
		return *this;
		;
	}

	Buffer &Buffer::operator>>(uint8_t &val) {
		val = _buffer[_position++];
		return *this;

	}

	Buffer &Buffer::operator<<(int8_t val) {
		_buffer.push_back(static_cast<uint8_t>(val));
		return *this;

	}

	Buffer &Buffer::operator>>(int8_t &val) {
		val = static_cast<uint8_t>(_buffer[_position++]);
		return *this;
	}

	Buffer &Buffer::operator<<(uint16_t val) {
		_buffer.push_back(static_cast<uint8_t>(val));
		_buffer.push_back(static_cast<uint8_t>(val >> 8));
		return *this;
	}

	Buffer &Buffer::operator>>(uint16_t &val) {
		val = static_cast<uint16_t>(_buffer[_position++]);
		val += static_cast<uint16_t>(_buffer[_position++] << 8);
		return *this;
	}

	Buffer &Buffer::operator<<(int16_t val) {
		return *this << static_cast<uint16_t >(val);;
	}

	Buffer &Buffer::operator>>(int16_t &val) {
		return *this >> reinterpret_cast<uint16_t&>(val);
	}

	Buffer &Buffer::operator<<(uint32_t val) {
		_buffer.push_back(static_cast<uint8_t>(val));
		_buffer.push_back(static_cast<uint8_t>(val >> 8));
		_buffer.push_back(static_cast<uint8_t>(val >> 16));
		_buffer.push_back(static_cast<uint8_t>(val >> 24));

		return *this;
	}

	Buffer &Buffer::operator>>(uint32_t &val) {
		val = static_cast<uint32_t>(_buffer[_position++]);
		val += static_cast<uint32_t>(_buffer[_position++] << 8);
		val += static_cast<uint32_t>(_buffer[_position++] << 16);
		val += static_cast<uint32_t>(_buffer[_position++] << 24);

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
		_buffer.insert(_buffer.end(), val.begin(), val.end());
		return *this;
	}

	Buffer &Buffer::operator>>(std::vector<uint8_t> &val) {
		std::copy(_buffer.begin() + _position, _buffer.begin() + _position + val.size(), val.begin());
		_position += val.size();
		return *this;
	}

	Buffer &Buffer::operator<<(const std::vector<uint16_t> &val) {
		for(auto& v : val) {
			_buffer.push_back(static_cast<uint8_t>(v));
			_buffer.push_back(static_cast<uint8_t>(v >> 8));
		}

		return *this;
	}

	Buffer &Buffer::operator>>(std::vector<uint16_t> &val) {
		for (auto& v : val) {
			v = static_cast<int16_t>(_buffer[_position++]);
			v += static_cast<int16_t>(_buffer[_position++] << 8);
		}

		return *this;
	}
}
}
