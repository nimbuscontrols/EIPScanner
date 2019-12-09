//
// Created by Aleksey Timin on 12/8/19.
//

#ifndef EIPSCANNER_IDENTITYOBJECT_H
#define EIPSCANNER_IDENTITYOBJECT_H

#include "cip/CipString.h"
#include "cip/CipRevision.h"
#include "cip/Types.h"
#include "SessionInfo.h"
#include "MessageRouter.h"
#include "BaseObject.h"

namespace eipScanner {
	class IdentityObject : public BaseObject {
	public:
		static const cip::CipUint CLASS_ID = 0x01;
		IdentityObject(cip::CipUint instanceId);
		IdentityObject(cip::CipUint instanceId, const SessionInfo::SPtr& si);
		IdentityObject(cip::CipUint instanceId, const SessionInfo::SPtr& si, const MessageRouter::SPtr& messageRouter);

		cip::CipUint getVendorId() const;
		cip::CipUint getDeviceType() const;
		cip::CipUint getProductCode() const;
		const cip::CipRevision &getRevision() const;
		cip::CipWord getStatus() const;
		cip::CipUdint getSerialNumber() const;
		std::string getProductName() const;

		void setVendorId(cip::CipUint vendorId);
		void setDeviceType(cip::CipUint deviceType);
		void setProductCode(cip::CipUint productCode);
		void setRevision(const cip::CipRevision &revision);
		void setStatus(cip::CipWord status);
		void setSerialNumber(cip::CipUdint serialNumber);
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
