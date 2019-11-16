//
// Created by Aleksey Timin  on 11/16/19.
//



#include "SessionInfo.h"
#include "utils/Logger.h"
#include "eip/EncapsPacket.h"
#include "eip/EncapsPacketFactory.h"

namespace eipScanner {

	using utils::Logger;
	using utils::LogLevel;
	using eip::EncapsPacket;
	using eip::EncapsPacketFactory;
	using eip::EncapsStatusCodes;


	SessionInfo::SessionInfo(const std::string &host, int port)
			:_socket{host, port, 504} {

		EncapsPacket packet = EncapsPacketFactory().createRegisterSessionPacket();
		packet = sendAndReceive(packet);

		if (packet.getStatusCode() != EncapsStatusCodes::SUCCESS) {
			throw std::runtime_error("Failed to register session in " +
				_socket.getHost() + ":" + std::to_string(_socket.getPort()));
		}

		_sessionHandle = packet.getSessionHandle();
		Logger(LogLevel::INFO) << "Registered session " << _sessionHandle;
	}

	SessionInfo::~SessionInfo() {
		EncapsPacket packet = EncapsPacketFactory().createUnRegisterSessionPacket(_sessionHandle);
		_socket.Send(packet.pack());
		Logger(LogLevel::INFO) << "Unregistered session " << _sessionHandle;
	}

	EncapsPacket SessionInfo::sendAndReceive(const EncapsPacket& packet) {
		_socket.Send(packet.pack());
		auto header = _socket.Receive(EncapsPacket::HEADER_SIZE);
		auto length = EncapsPacket::GetLengthFromHeader(header);
		auto data = _socket.Receive(length);

		header.insert(header.end(), data.begin(), data.end());

		EncapsPacket recvPacket;
		recvPacket.expand(header);

		//TODO: The status of the packet must be checked
		return recvPacket;
	}
}