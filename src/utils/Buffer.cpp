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

	Buffer &Buffer::operator<<(uint64_t val) {
		_buffer.push_back(static_cast<uint8_t>(val));
		_buffer.push_back(static_cast<uint8_t>(val >> 8));
		_buffer.push_back(static_cast<uint8_t>(val >> 16));
		_buffer.push_back(static_cast<uint8_t>(val >> 24));
		_buffer.push_back(static_cast<uint8_t>(val >> 32));
		_buffer.push_back(static_cast<uint8_t>(val >> 40));
		_buffer.push_back(static_cast<uint8_t>(val >> 48));
		_buffer.push_back(static_cast<uint8_t>(val >> 56));

		return *this;
	}

	Buffer &Buffer::operator>>(uint64_t &val) {
		val = static_cast<uint64_t>(_buffer[_position++]);
		val += static_cast<uint64_t>(_buffer[_position++]) << 8;
		val += static_cast<uint64_t>(_buffer[_position++]) << 16;
		val += static_cast<uint64_t>(_buffer[_position++]) << 24;
		val += static_cast<uint64_t>(_buffer[_position++]) << 32;
		val += static_cast<uint64_t>(_buffer[_position++]) << 40;
		val += static_cast<uint64_t>(_buffer[_position++]) << 48;
		val += static_cast<uint64_t>(_buffer[_position++]) << 56;

		return *this;
	}

	Buffer &Buffer::operator<<(int64_t val) {
		return *this << static_cast<uint64_t >(val);;
	}

	Buffer &Buffer::operator>>(int64_t &val) {
		return *this >> reinterpret_cast<uint64_t&>(val);
	}

Buffer &Buffer::operator<<(float val) {
		return *this << reinterpret_cast<uint32_t&>(val);
	}

	Buffer &Buffer::operator>>(float &val)  {
		return *this >> reinterpret_cast<uint32_t&>(val);
	}

	Buffer &Buffer::operator<<(double val) {
		return *this << reinterpret_cast<uint64_t&>(val);
	}

	Buffer &Buffer::operator>>(double &val) {
		return *this >> reinterpret_cast<uint64_t&>(val);
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

	Buffer &Buffer::operator<<(cip::CipRevision val) {
		return *this << val.getMajorRevision() << val.getMinorRevision();
	}

	Buffer &Buffer::operator>>(cip::CipRevision &val) {
		cip::CipUsint majorRevision, minorRevision;
		*this >> majorRevision >> minorRevision;
		val = cip::CipRevision(majorRevision, minorRevision);
		return *this;
	}

	Buffer &Buffer::operator<<(sockets::EndPoint v) {
		std::vector<uint8_t> zeros(8);
		sockaddr_in addr = v.getAddr();
		return *this << htons(static_cast<cip::CipInt>(addr.sin_family))
					 << addr.sin_port
					 << uint32_t(addr.sin_addr.s_addr)
					 << zeros;
	}

	Buffer &Buffer::operator>>(sockets::EndPoint &val) {
		std::vector<uint8_t> zeros(8);
		sockaddr_in addr{0};
		*this >> reinterpret_cast<cip::CipInt&>(addr.sin_family)
			 >> addr.sin_port
			 >> (uint32_t&)addr.sin_addr.s_addr
			 >> zeros;

		addr.sin_family =  htons(addr.sin_family);
		val = sockets::EndPoint(addr);
		return *this;
	}
}
}
