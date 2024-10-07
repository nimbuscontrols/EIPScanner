//
// Created by Aleksey Timin on 12/17/19.
//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include <EIPScanner/DiscoveryManager.h>
#include <EIPScanner/utils/Logger.h>

using eipScanner::DiscoveryManager;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

int main() {
  Logger::setLogLevel(LogLevel::DEBUG);

#if OS_Windows
  WSADATA wsaData;
  int winsockStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (winsockStart != 0) {
    Logger(LogLevel::ERROR) << "Failed to start WinSock - error code: " << winsockStart;
    return EXIT_FAILURE;
  }
#endif

  DiscoveryManager discoveryManager("172.28.255.255", 0xAF12, std::chrono::seconds(1));
  auto devices = discoveryManager.discover();

  for (auto& device : devices) {
    Logger(LogLevel::INFO) << "Discovered device: "
      << device.identityObject.getProductName()
      << " with address " << device.socketAddress.toString();
  }

#if OS_Windows
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
