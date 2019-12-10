//
// Created by Aleksey Timin on 12/10/19.
//

#ifndef EIPSCANNER_SESSIONINFOIF_H
#define EIPSCANNER_SESSIONINFOIF_H

#include "eip/EncapsPacket.h"
namespace eipScanner {

	class SessionInfoIf {
	public:
		using SPtr = std::shared_ptr<SessionInfoIf>;

		virtual eip::EncapsPacket sendAndReceive(const eip::EncapsPacket &packet) const = 0;

		virtual cip::CipUdint getSessionHandle() const = 0;

		virtual std::string getHost() const = 0;
	};
}
#endif //EIPSCANNER_SESSIONINFOIF_H
