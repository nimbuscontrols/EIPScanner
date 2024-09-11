//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_MESSAGEROUTER_H
#define EIPSCANNER_MESSAGEROUTER_H

#include <memory>
#include "src/cip/EPath.h"
#include "src/cip/Services.h"
#include "src/cip/MessageRouterResponse.h"
#include "src/eip/CommonPacketItem.h"
#include "src/SessionInfo.h"

namespace eipScanner {
	/**
	* @class MessageRouter
	*
	* @brief Implements the explicit messaging with EIP adapter
	*/
	class MessageRouter {
	public:
		using SPtr = std::shared_ptr<MessageRouter>;

        static constexpr bool USE_8_BIT_PATH_SEGMENTS = true;

		/**
		 * @brief Default constructor
		 */
		MessageRouter(bool use_8_bit_path_segments=false);

		/**
		 * @brief Default destructor
		 */
		virtual ~MessageRouter();

		/**
		 * @brief Sends an explicit requests to the EIP adapter by calling a CIP service
		 * @param si the EIP session with the adapter
		 * @param service the service code (for standard codes see eipScanner::cip::ServiceCodes)
		 * @param path the path to an element in Object Model that provides the called service
		 * @param data the encoded arguments of the service
		 * @param additionalPacketItems (needed only for eipScanner::ConnectionManager)
		 * @return the received response from the EIP adapter
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
				const cip::EPath& path, const std::vector<uint8_t>& data,
				const std::vector<eip::CommonPacketItem>& additionalPacketItems) const;

		/**
		 * @brief Sends an explicit requests to the EIP adapter by calling a CIP service
		 * @param si the EIP session with the adapter
		 * @param service the service code (for standard codes see eipScanner::cip::ServiceCodes)
		 * @param path the path to an element in Object Model that provides the called service
		 * @param data the encoded arguments of the service
		 * @return the received response from the EIP adapter
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
				const cip::EPath& path, const std::vector<uint8_t>& data) const;

		/**
		 * @brief Sends an explicit requests to the EIP adapter by calling a CIP service
		 * @param si the EIP session with the adapter
		 * @param service the service code (for standard codes see eipScanner::cip::ServiceCodes)
		 * @param path the path to an element in Object Model that provides the called service
		 * @return the received response from the EIP adapter
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
													   const cip::EPath& path) const;

    private:
        bool _use_8_bit_path_segments;
	};
}


#endif  // EIPSCANNER_MESSAGEROUTER_H
