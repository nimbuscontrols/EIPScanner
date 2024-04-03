//
// Created by Aleksey Timin on 12/10/19.
//

#ifndef EIPSCANNER_SESSIONINFOIF_H
#define EIPSCANNER_SESSIONINFOIF_H

#include <memory>
#include "eip/EncapsPacket.h"
#include "sockets/EndPoint.h"

namespace eipScanner {
	/**
	* @class SessionInfoIf
	*
	* @brief Abstract interface for EIP session
	*/
	class SessionInfoIf {
	public:
		using SPtr = std::shared_ptr<SessionInfoIf>;

		/**
		 * Sends and receives EIP Encapsulation packet
		 * @param packet the EIP Encapsulation packet to send
		 * @return the received EIP Encapsulation packet
		 */
		virtual eip::EncapsPacket sendAndReceive(const eip::EncapsPacket &packet) const = 0;

		/**
		 * Gets the handle of the current EIP session
		 * @return
		 */
		virtual cip::CipUdint getSessionHandle() const = 0;

		/**
		 * Gets the address of the EIP adapter which the session is established with
		 * @return
		 */
		virtual sockets::EndPoint getRemoteEndPoint() const = 0;

		/**
		 * Gets whether this connection should use 8-bit path segments.
		 * @return
		 */
		virtual bool getUse8BitPathSegments() const = 0;
	};
}
#endif //EIPSCANNER_SESSIONINFOIF_H
