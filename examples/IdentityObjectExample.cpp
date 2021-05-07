//
// Created by Aleksey Timin on 12/19/19.
//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include "IdentityObject.h"
#include "utils/Logger.h"

using eipScanner::IdentityObject;
using eipScanner::SessionInfo;
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

  auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);
  IdentityObject identityObject(1, si);

  Logger(LogLevel::INFO) << identityObject.getVendorId()
      << identityObject.getDeviceType()
      << identityObject.getProductCode()
      << identityObject.getRevision().toString()
      << identityObject.getStatus()
      << identityObject.getSerialNumber()
      << identityObject.getProductName();

#if OS_Windows
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
