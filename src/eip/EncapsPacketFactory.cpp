//
// Created by Aleksey Timin on 11/16/19.
//

#include <utils/Buffer.h>
#include "EncapsPacketFactory.h"

namespace eipScanner {
namespace eip {

	using utils::Buffer;

	EncapsPacket EncapsPacketFactory::createRegisterSessionPacket() const {
		EncapsPacket packet;
		packet.setCommand(EncapsCommands::REGISTER_SESSION);

		// See 2-4.4 Vol2 spec
		cip::CipUint protocolVersion = 1;
		cip::CipUint optionFlag = 0;

		Buffer buffer(4);
		buffer << protocolVersion << optionFlag;
		packet.setData(buffer.data());

		return packet;
	}

	EncapsPacket EncapsPacketFactory::createUnRegisterSessionPacket(cip::CipUdint sessionHandle) const {
		EncapsPacket packet;
		packet.setCommand(EncapsCommands::UN_REGISTER_SESSION);
		packet.setSessionHandle(sessionHandle);
		return packet;
	}

	EncapsPacket EncapsPacketFactory::createSendRRDataPacket(cip::CipUdint sessionHandle, cip::CipUint timeout,
															 std::vector<uint8_t> data) const {

		EncapsPacket packet;
		packet.setCommand(EncapsCommands::SEND_RR_DATA);
		packet.setSessionHandle(sessionHandle);

		cip::CipUdint interfaceHandle = 0;
		Buffer buffer(6 + data.size());
		buffer << interfaceHandle << timeout << data;
		packet.setData(buffer.data());

		return packet;
	}

	EncapsPacket EncapsPacketFactory::createListIdentityPacket() const {
		EncapsPacket packet;
		packet.setCommand(EncapsCommands::LIST_IDENTITY);
		packet.setSessionHandle(0);
		return packet;
	}
}
}
