//
// Created by Aleksey Timin on 11/16/19.
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

	SessionInfo::SessionInfo(const std::string &host, int port, const std::chrono::milliseconds &timeout)
			: _socket{sockets::EndPoint(host, port), timeout}
			, _sessionHandle{0} {
		_socket.setRecvTimeout(timeout);

		EncapsPacket packet = EncapsPacketFactory().createRegisterSessionPacket();
		packet = sendAndReceive(packet);

		if (packet.getStatusCode() != EncapsStatusCodes::SUCCESS) {
			throw std::runtime_error("Failed to register session in " +
									 _socket.getRemoteEndPoint().toString());
		}

		_sessionHandle = packet.getSessionHandle();
		Logger(LogLevel::INFO) << "Registered session " << _sessionHandle;
	}

	SessionInfo::SessionInfo(const std::string &host, int port)
			: SessionInfo(host, port, std::chrono::milliseconds(1000)) {
	}

	SessionInfo::~SessionInfo() {
		EncapsPacket packet = EncapsPacketFactory().createUnRegisterSessionPacket(_sessionHandle);
		_socket.Send(packet.pack());
		Logger(LogLevel::INFO) << "Unregistered session " << _sessionHandle;
	}

	EncapsPacket SessionInfo::sendAndReceive(const EncapsPacket& packet) const {
		_socket.Send(packet.pack());
		auto header = _socket.Receive(EncapsPacket::HEADER_SIZE);
		auto length = EncapsPacket::getLengthFromHeader(header);
		auto data = _socket.Receive(length);

		header.insert(header.end(), data.begin(), data.end());

		EncapsPacket recvPacket;
		recvPacket.expand(header);

		if (recvPacket.getStatusCode() != EncapsStatusCodes::SUCCESS) {
			throw std::runtime_error("Bad encaps packet code =" + std::to_string(
					static_cast<int>(recvPacket.getStatusCode())));
		}

		if (_sessionHandle != 0 && recvPacket.getSessionHandle() != _sessionHandle) {
			throw std::runtime_error("Wrong session handle received");
		}


		return recvPacket;
	}

	cip::CipUdint SessionInfo::getSessionHandle() const {
		return _sessionHandle;
	}

	sockets::EndPoint SessionInfo::getRemoteEndPoint() const {
		return _socket.getRemoteEndPoint();
	}

}
