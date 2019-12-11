//
// Created by Aleksey Timin on 12/10/19.
//

#ifndef EIPSCANNER_SESSIONINFOIF_H
#define EIPSCANNER_SESSIONINFOIF_H

#include <memory>
#include "eip/EncapsPacket.h"
#include "sockets/EndPoint.h"

namespace eipScanner {

	class SessionInfoIf {
	public:
		using SPtr = std::shared_ptr<SessionInfoIf>;

		virtual eip::EncapsPacket sendAndReceive(const eip::EncapsPacket &packet) const = 0;

		virtual cip::CipUdint getSessionHandle() const = 0;

		virtual sockets::EndPoint getRemoteEndPoint() const = 0;
	};
}
#endif //EIPSCANNER_SESSIONINFOIF_H
