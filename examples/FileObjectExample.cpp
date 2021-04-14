//
// Created by Aleksey Timin on 11/24/19.
//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include <fstream>
#include "FileObject.h"
#include "utils/Logger.h"
#include "fileObject/FileObjectState.h"

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;
using eipScanner::FileObject;

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

  FileObject edsFile(0xC8, si);
  edsFile.beginUpload(si, [](GeneralStatusCodes status, const std::vector<uint8_t> &fileContent) {
    if (status == GeneralStatusCodes::SUCCESS) {
      std::ofstream outFile("Device.eds", std::ios::out | std::ios::trunc | std::ios::binary);
      outFile.write(reinterpret_cast<const char *>(fileContent.data()), fileContent.size());
    }
  });

  while (edsFile.handleTransfers(si)) {
    continue;
  };

#if OS_Windows
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
