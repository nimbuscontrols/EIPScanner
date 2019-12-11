//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef _SRC_SESSIONINFO_H_
#define _SRC_SESSIONINFO_H_

#include <string>
#include <vector>
#include <memory>

#include "SessionInfoIf.h"
#include "sockets/TCPSocket.h"

namespace eipScanner {
	class SessionInfo : public SessionInfoIf {
	public:
		using SPtr = std::shared_ptr<SessionInfo>;

		SessionInfo(const std::string &host, int port, const std::chrono::milliseconds& recvTimeout);
		SessionInfo(const std::string &host, int port);
		~SessionInfo();

		eip::EncapsPacket sendAndReceive(const eip::EncapsPacket& packet) const override;
		cip::CipUdint getSessionHandle() const override;
		sockets::EndPoint getRemoteEndPoint() const override;

	private:
		sockets::TCPSocket _socket;
		cip::CipUdint _sessionHandle;

	};
}

#endif  // _SRC_SESSIONINFO_H_
