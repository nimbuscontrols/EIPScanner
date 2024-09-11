//
// Created by Aleksey Timin on 12/8/19.
//

#ifndef EIPSCANNER_IDENTITYOBJECT_H
#define EIPSCANNER_IDENTITYOBJECT_H

#include "src/cip/CipString.h"
#include "src/cip/CipRevision.h"
#include "src/cip/Types.h"
#include "src/SessionInfo.h"
#include "src/MessageRouter.h"
#include "src/BaseObject.h"

namespace eipScanner {
	/**
	* @class IdentityObject
	*
	* @brief Implements interface to Identity Object (0x01).
	*
	* It reads all data from the CIP instance in the constructor
	*/
	class IdentityObject : public BaseObject {
	public:
		static const cip::CipUint CLASS_ID = 0x01;

		/**
		 * @brief Creates an empty instance without any EIP requests
		 * @param instanceId the ID of the CIP instance
		 */
		IdentityObject(cip::CipUint instanceId);

		/**
		 * @brief Creates an instance and reads all its data via EIP
		 * @param instanceId the ID of the CIP instance
		 * @param si the EIP session for explicit messaging
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		IdentityObject(cip::CipUint instanceId, const SessionInfoIf::SPtr& si);

		/**
		 * @brief Creates an instance and reads all its data via EIP
	 	 * @note Used for testing
		 * @param instanceId
		 * @param si
		 * @param messageRouter
		 */
		IdentityObject(cip::CipUint instanceId, const SessionInfoIf::SPtr& si, const MessageRouter::SPtr& messageRouter);

		/**
		 * @brief Gets Vendor ID [AttrID=1]
		 * @return
		 */
		cip::CipUint getVendorId() const;

		/**
		 * @brief Gets Device Type [AttrID=2]
		 * @return
		 */
		cip::CipUint getDeviceType() const;

		/**
		 * @brief Gets Product Code [AttrID=3]
		 * @return
		 */
		cip::CipUint getProductCode() const;

		/**
		 * @brief Gets Revision [AttrID=4]
		 * @return
		 */
		const cip::CipRevision &getRevision() const;

		/**
		 * @brief Gets Status [AttrID=5]
		 * @return
		 */
		cip::CipWord getStatus() const;

		/**
		 * @brief Gets Serial Number [AttrID=6]
		 * @return
		 */
		cip::CipUdint getSerialNumber() const;

		/**
		 * @brief Gets Product Name [AttrID=7]
		 * @return
		 */
		std::string getProductName() const;

		/**
		 * @brief Sets Vendor ID [AttrID=1]
		 * @param vendorId
		 */
		void setVendorId(cip::CipUint vendorId);

		/**
		 * @brief Sets Device Type [AttrID=2]
		 * @param deviceType
		 */
		void setDeviceType(cip::CipUint deviceType);

		/**
		 * @brief Sets Product Code [AttrID=3]
		 * @param productCode
		 */
		void setProductCode(cip::CipUint productCode);

		/**
		 * @brief Sets Revision [AttrID=4]
		 * @param revision
		 */
		void setRevision(const cip::CipRevision &revision);

		/**
		 * @brief Sets Status [AttrID=5]
		 * @param status
		 */
		void setStatus(cip::CipWord status);

		/**
		 * @brief Sets Serial Number [AttrID=6]
		 * @param serialNumber
		 */
		void setSerialNumber(cip::CipUdint serialNumber);

		/**
		 * @brief Sets Product Name [AttrID=7]
		 * @param productName
		 */
		void setProductName(const std::string &productName);

	private:
		cip::CipUint _vendorId;
		cip::CipUint _deviceType;
		cip::CipUint _productCode;
		cip::CipRevision _revision;
		cip::CipWord _status;
		cip::CipUdint _serialNumber;
		cip::CipShortString _productName;
	};
}

#endif  //EIPSCANNER_IDENTITYOBJECT_H
