//
// Created by flipback on 11/16/19.
//

#include <string>
#include <stdexcept>
#include "EncapsPacket.h"
#include "utils/Buffer.h"

using eipScanner::utils::Buffer;

namespace eipScanner {
namespace eip {

	EncapsPacket::EncapsPacket() : _command{EncapsCommands::NOP}
		, _length{0}
		, _sessionHandle{0}
		, _statusCode{EncapsStatusCodes::SUCCESS}
		, _context(8)
		, _options{0}
		, _data(0) {
	}

	EncapsPacket::~EncapsPacket() = default;

	void EncapsPacket::expand(const std::vector<uint8_t> &data) {
		if (data.size() < HEADER_SIZE) {
			throw std::runtime_error("EncapsPacket header must be 24 bytes");
		}
		
		Buffer buffer(data);
		buffer >> reinterpret_cast<cip::CipUint&>(_command)
			>> _length
			>> _sessionHandle
			>> reinterpret_cast<cip::CipUdint&>(_statusCode)
			>> _context
			>> _options;

		auto dataSize = data.size() - HEADER_SIZE;
		if (dataSize != _length) {
			throw std::runtime_error("EncapsPacket data must be "  + std::to_string(_length)
				+ " but we have only " + std::to_string(dataSize) + " bytes");
		}
		
		_data.resize(_length);
		buffer >> _data;
	}

	std::vector<uint8_t> EncapsPacket::pack() const {
		Buffer buffer;

		buffer << static_cast<cip::CipUint>(_command)
			<< _length
			<< _sessionHandle
			<< static_cast<cip::CipUdint>(_statusCode)
			<< _context
			<< _options
			<< _data;

		return buffer.data();
	}

	EncapsCommands EncapsPacket::getCommand() const {
		return _command;
	}

	void EncapsPacket::setCommand(EncapsCommands command) {
		_command = command;
	}

	cip::CipUint EncapsPacket::getLength() const {
		return _length;
	}

	cip::CipUdint EncapsPacket::getSessionHandle() const {
		return _sessionHandle;
	}

	void EncapsPacket::setSessionHandle(cip::CipUdint sessionHandle) {
		_sessionHandle = sessionHandle;
	}


	EncapsStatusCodes EncapsPacket::getStatusCode() const {
		return _statusCode;
	}

	void EncapsPacket::setStatusCode(EncapsStatusCodes statusCode) {
		_statusCode = statusCode;
	}

	const std::vector<uint8_t> &EncapsPacket::getData() const {
		return _data;
	}

	void EncapsPacket::setData(const std::vector<uint8_t> &data) {
		_data = data;
		_length = data.size();
	}

	size_t EncapsPacket::GetLengthFromHeader(const std::vector<uint8_t> data) {
		std::vector<uint8_t> lengthVector(data.begin() + 2, data.begin() + 4);
		Buffer buf(lengthVector);

		cip::CipUint len;
		buf >> len;
		return len;
	}
}
}
