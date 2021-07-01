//
// Created by Aleksey Timin on 11/18/19.
//

#ifndef EIPSCANNER_CONNECTIONMANAGER_H
#define EIPSCANNER_CONNECTIONMANAGER_H

#include <map>
#include "MessageRouter.h"
#include "IOConnection.h"
#include "cip/connectionManager/ConnectionParameters.h"
#include "cip/Services.h"
#include "cip/Types.h"
#include "sockets/UDPBoundSocket.h"

namespace eipScanner {
	/**
	* @class ConnectionManager
	*
	* @brief Implements the implicit messaging with EIP adapter
	*/
	class ConnectionManager {
	public:
		/**
		 * @brief Default constructor
		 */
		ConnectionManager();

		 /**
		 * @brief Construct a ConnectionManager using a non-default bind port.
		 * This is usefull for testing if port 2222 is already in use by someone else.
		 */
		ConnectionManager(int bindPort);

		/**
		 * @note used fot testing
		 * @param messageRouter
		 */
		explicit ConnectionManager(const MessageRouter::SPtr& messageRouter);

		/**
		 * @brief Default destructor
		 */
		~ConnectionManager();

		/**
		 * @brief Opens an EIP IO connection with the EIP adapter
		 * @param si the EIP session for explicit messaging
		 * @param connectionParameters the parameters of the connection
		 * @param isLarge use large forward open if true
		 * @return weak pointer to the created connection or nullptr if got an error
		 */
		IOConnection::WPtr forwardOpen(const SessionInfoIf::SPtr& si, cip::connectionManager::ConnectionParameters connectionParameters, bool isLarge = false);

		/**
		 * @brief Opens an EIP IO connection with the EIP adapter
		 * @param si the EIP session for explicit messaging
		 * @param connectionParameters the parameters of the connection
		 * @return weak pointer to the created connection or nullptr if got an error
		 */
		IOConnection::WPtr largeForwardOpen(const SessionInfoIf::SPtr& si, cip::connectionManager::ConnectionParameters connectionParameters);


		/**
		 * @brief Closes an EIP IO connection
		 * @param si the EIP session for explicit messaging
		 * @param ioConnection the connection to close
		 */
		void forwardClose(const SessionInfoIf::SPtr& si, const IOConnection::WPtr& ioConnection);

		/**
		 * @brief Handles active connections
		 * @param timeout the timeout of receiving the data by select function
		 */
		void handleConnections(std::chrono::milliseconds timeout);

		/**
		 *
		 * @return true if there are some opened IO connections
		 */
		bool hasOpenConnections() const;
	private:
		MessageRouter::SPtr _messageRouter;
		std::map<cip::CipUint, IOConnection::SPtr> _connectionMap;
		std::map<sockets::EndPoint, std::shared_ptr<sockets::UDPBoundSocket>> _socketMap;

		sockets::UDPBoundSocket::SPtr  findOrCreateSocket(const sockets::EndPoint& endPoint);
		cip::CipUint _incarnationId;
		int _bindPort = 2222;
	};
}

#endif  // EIPSCANNER_CONNECTIONMANAGER_H
