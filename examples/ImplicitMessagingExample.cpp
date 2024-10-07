//
// Created by Aleksey Timin on 11/16/19.
//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include <sstream>
#include <EIPScanner/cip/connectionManager/NetworkConnectionParams.h>
#include <EIPScanner/SessionInfo.h>
#include <EIPScanner/ConnectionManager.h>
#include <EIPScanner/utils/Logger.h>
#include <EIPScanner/utils/Buffer.h>

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::ConnectionManager;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;
using eipScanner::utils::Buffer;
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

  // Implicit messaging
  ConnectionManager connectionManager;

  ConnectionParameters parameters;
  parameters.connectionPath = {0x20, 0x04,0x24, 151, 0x2C, 150, 0x2C, 100};  // config Assm151, output Assm150, intput Assm100
  parameters.o2tRealTimeFormat = true;
  parameters.originatorVendorId = 342;
  parameters.originatorSerialNumber = 32423;
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.t2oNetworkConnectionParams |= 32; //size of Assm100 =32
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.o2tNetworkConnectionParams |= 32; //size of Assm150 = 32

  parameters.originatorSerialNumber = 0x12345;
  parameters.o2tRPI = 1000000;
  parameters.t2oRPI = 1000000;
  parameters.transportTypeTrigger |= NetworkConnectionParams::CLASS1;

  auto io = connectionManager.forwardOpen(si, parameters);
  if (auto ptr = io.lock()) {
    ptr->setDataToSend(std::vector<uint8_t>(32));

    ptr->setReceiveDataListener([](auto realTimeHeader, auto sequence, auto data) {
      std::ostringstream ss;
      ss << "secNum=" << sequence << " data=";
      for (auto &byte : data) {
        ss << "[" << std::hex << (int) byte << "]";
      }

      Logger(LogLevel::INFO) << "Received: " << ss.str();
    });

    ptr->setCloseListener([]() {
      Logger(LogLevel::INFO) << "Closed";
    });
  }

  int count = 200;
  while (connectionManager.hasOpenConnections() && count-- > 0) {
    connectionManager.handleConnections(std::chrono::milliseconds(100));
  }

  connectionManager.forwardClose(si, io);

#if OS_Windows
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
