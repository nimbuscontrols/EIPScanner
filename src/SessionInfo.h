//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef _SRC_SESSIONINFO_H_
#define _SRC_SESSIONINFO_H_

#include <string>
#include <vector>

#include "eip/EncapsPacket.h"
#include "sockets/TCPSocket.h"

namespace eipScanner {
	class SessionInfo {
	public:
		SessionInfo(const std::string &host, int port);
		~SessionInfo();
		virtual eip::EncapsPacket sendAndReceive(const eip::EncapsPacket& packet);
	private:
		sockets::TCPSocket _socket;
		cip::CipUdint _sessionHandle;

	};
}

#endif  // _SRC_SESSIONINFO_H_
