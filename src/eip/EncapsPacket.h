//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_EIP_ENCAPSPACKET_H
#define EIPSCANNER_EIP_ENCAPSPACKET_H

#include <cstdint>
#include <vector>

#include "cip/Types.h"
#include <vector>

namespace eipScanner {
namespace eip {
	enum class EncapsCommands : cip::CipUint {
		NOP = 0,
		LIST_SERVICES = 0x0004,
		LIST_IDENTITY = 0x0063,
		LIST_INTERFACES = 0x0064,
		REGISTER_SESSION = 0x0065,
		UN_REGISTER_SESSION =0x0066,
		SEND_RR_DATA = 0x006F,
		SEND_UNIT_DATA = 0x0070,
		INDICATE_STATUS = 0x0072,
		CANCEL = 0x0073
	};

	enum class EncapsStatusCodes : cip::CipUdint {
		SUCCESS = 0x0000,
		UNSUPPORTED_COMMAND = 0x0001,
		INSUFFICIENT_MEMORY = 0x0002,
		INVALID_FORMAT_OR_DATA = 0x0003,
		INVALID_SESSION_HANDLE = 0x0064,
		UNSUPPORTED_PROTOCOL_VERSION = 0x0069
	};

	class EncapsPacket {
	public:
		EncapsPacket();
		~EncapsPacket();
		std::vector<uint8_t> pack() const;
		void expand(const std::vector<uint8_t>& data);

		// Getters & setters
		EncapsCommands getCommand() const;
		void setCommand(EncapsCommands command);

		cip::CipUint getLength() const;

		cip::CipUdint getSessionHandle() const;
		void setSessionHandle(cip::CipUdint sessionHandle);

		EncapsStatusCodes getStatusCode() const;
		void setStatusCode(EncapsStatusCodes statusCode);

		const std::vector<uint8_t> &getData() const;
		void setData(const std::vector<uint8_t> &data);

	private:
		EncapsCommands _command;
		cip::CipUint _length;
		cip::CipUdint _sessionHandle;
		EncapsStatusCodes _statusCode;
		std::vector<uint8_t> _context;
		cip::CipUdint _options;
		std::vector<uint8_t> _data;
	};
}
}


#endif  // EIPSCANNER_EIP_ENCAPSPACKET_H
