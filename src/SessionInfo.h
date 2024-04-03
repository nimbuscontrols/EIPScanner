//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef _SRC_SESSIONINFO_H_
#define _SRC_SESSIONINFO_H_

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

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
		 * @param timeout timeout to connect and receive the response
		 * @param use_8_bit_path_segments use 8-bit instead of 16-bit path segments
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		SessionInfo(const std::string &host, int port, const std::chrono::milliseconds& timeout, bool use_8_bit_path_segments);

		/**
		 * @brief Establishes an EIP session with an EIP adapter
		 * @param host The IP address of the adapter
		 * @param port The port of the adapter
		 * @param use_8_bit_path_segments use 8-bit instead of 16-bit path segments
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		SessionInfo(const std::string &host, int port, bool use_8_bit_path_segments);

		/**
		 * @brief Establishes an EIP session with an EIP adapter
		 * @param host The IP address of the adapter
		 * @param port The port of the adapter
		 * @param timeout timeout to connect and receive the response
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		SessionInfo(const std::string &host, int port, const std::chrono::milliseconds& timeout);

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

		/**
		 * Gets whether this connection should use 8-bit path segments.
		 * @return
		 */
		bool getUse8BitPathSegments() const override;

	private:
		sockets::TCPSocket _socket;
		cip::CipUdint _sessionHandle;
		bool _use_8_bit_path_segments;
	};
}

#endif  // _SRC_SESSIONINFO_H_
