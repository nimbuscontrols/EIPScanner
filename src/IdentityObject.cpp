//
// Created by Aleksey Timin on 12/8/19.
//

#include "IdentityObject.h"
#include "utils/Buffer.h"

namespace eipScanner {
	using namespace cip;
	using utils::Buffer;
	
	IdentityObject::IdentityObject(cip::CipUint instanceId)
		: BaseObject(CLASS_ID, instanceId)
		, _vendorId(0)
		, _deviceType(0)
		, _productCode(0)
		, _revision(0,0)
		, _status(0)
		, _serialNumber(0)
		, _productName("") {
	}

	IdentityObject::IdentityObject(cip::CipUint instanceId, const SessionInfo::SPtr &si)
		: IdentityObject(instanceId, si, std::make_shared<MessageRouter>()){
	}

	IdentityObject::IdentityObject(cip::CipUint instanceId, const SessionInfo::SPtr &si, const MessageRouter::SPtr &messageRouter)
		: BaseObject(CLASS_ID, instanceId) {

		auto response = messageRouter->sendRequest(
				si,
				ServiceCodes::GET_ATTRIBUTE_ALL,
				EPath(CLASS_ID, 1),
				{});

		if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
			Buffer buffer(response.getData());
			buffer >> _vendorId >> _deviceType
				>> _productCode >> _revision
				>> _status >> _serialNumber
				>> _productName;

			if (!buffer.isValid()) {
				std::runtime_error("Not enough data in the response");
			}

		} else {
			logGeneralAndAdditionalStatus(response);
			throw std::runtime_error("Failed to read all attributes");
		}
	}

	CipUint IdentityObject::getVendorId() const {
		return _vendorId;
	}

	void IdentityObject::setVendorId(CipUint vendorId) {
		_vendorId = vendorId;
	}

	CipUint IdentityObject::getDeviceType() const {
		return _deviceType;
	}

	void IdentityObject::setDeviceType(CipUint deviceType) {
		_deviceType = deviceType;
	}

	CipUint IdentityObject::getProductCode() const {
		return _productCode;
	}

	void IdentityObject::setProductCode(CipUint productCode) {
		_productCode = productCode;
	}

	const CipRevision &IdentityObject::getRevision() const {
		return _revision;
	}

	void IdentityObject::setRevision(const CipRevision &revision) {
		_revision = revision;
	}

	CipWord IdentityObject::getStatus() const {
		return _status;
	}

	void IdentityObject::setStatus(CipWord status) {
		_status = status;
	}

	CipUdint IdentityObject::getSerialNumber() const {
		return _serialNumber;
	}

	void IdentityObject::setSerialNumber(CipUdint serialNumber) {
		_serialNumber = serialNumber;
	}

	std::string IdentityObject::getProductName() const {
		return _productName.toStdString();
	}

	void IdentityObject::setProductName(const std::string &productName) {
		_productName = CipShortString(productName);
	}
}