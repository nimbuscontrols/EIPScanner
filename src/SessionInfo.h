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
	/**
	* @class SessionInfo
	*
	* @brief Implementation of EIP session
	*/
	class SessionInfo : public SessionInfoIf {
	public:
		using SPtr = std::shared_ptr<SessionInfo>;

		/**
		 * @brief Establishes an EIP session with an EIP adapter
		 * @param host The IP address of the adapter
		 * @param port The port of the adapter
		 * @param recvTimeout timout to receive the response
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		SessionInfo(const std::string &host, int port, const std::chrono::milliseconds& recvTimeout);

		/**
		 * @brief Establishes an EIP session with an EIP adapter
		 * @param host The IP address of the adapter
		 * @param port The port of the adapter
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		SessionInfo(const std::string &host, int port);

		/**
		 * @brief Default destructor
		 */
		~SessionInfo();

		/**
		 * @sa SessionInfo::sendAndReceive
		 * @param packet
		 * @return
		 */
		eip::EncapsPacket sendAndReceive(const eip::EncapsPacket& packet) const override;

		/**
		 * @sa SessionInfo::getSessionHandle
		 * @return
		 */
		cip::CipUdint getSessionHandle() const override;

		/**
		 * @sa SessionInfo::getRemoteEndPoint
		 * @return
		 */
		sockets::EndPoint getRemoteEndPoint() const override;

	private:
		sockets::TCPSocket _socket;
		cip::CipUdint _sessionHandle;

	};
}

#endif  // _SRC_SESSIONINFO_H_
