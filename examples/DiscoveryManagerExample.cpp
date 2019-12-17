//
// Created by Aleksey Timin on 12/17/19.
//

#include <DiscoveryManager.h>
#include <utils/Logger.h>

using eipScanner::DiscoveryManager;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

int main() {
	Logger::setLogLevel(LogLevel::DEBUG);

	DiscoveryManager discoveryManager("172.28.255.255", 0xAF12, std::chrono::seconds(1));
	auto devices = discoveryManager.discover();

	for (auto& device : devices) {
		Logger(LogLevel::INFO) << "Discovered device: "
			<< device.identityObject.getProductName()
			<< " with address " << device.socketAddress.toString();
	}
}