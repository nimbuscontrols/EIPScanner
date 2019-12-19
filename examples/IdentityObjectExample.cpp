//
// Created by Aleksey Timin on 12/19/19.
//

#include "IdentityObject.h"
#include "utils/Logger.h"

using eipScanner::IdentityObject;
using eipScanner::SessionInfo;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

int main() {
	auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);
	IdentityObject identityObject(1, si);

	Logger(LogLevel::INFO) << identityObject.getVendorId()
			<< identityObject.getDeviceType()
			<< identityObject.getProductCode()
			<< identityObject.getRevision().toString()
			<< identityObject.getStatus()
			<< identityObject.getSerialNumber()
			<< identityObject.getProductName();

	return 0;
}