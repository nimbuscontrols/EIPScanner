//
// Created by Aleksey Timin on 12/4/19.
//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include "ParameterObject.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;
using eipScanner::ParameterObject;
using eipScanner::utils::Buffer;

const CipUint MAX_INSTANCE = 2;
const CipUint CLASS_DESCRIPTOR = 8;
const CipUint SUPPORTS_FULL_ATTRIBUTES = 0x2;

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

  // Read the number of the parameters
  MessageRouter messageRouter;
  auto response = messageRouter.sendRequest(si
      , ServiceCodes::GET_ATTRIBUTE_SINGLE
      , EPath(ParameterObject::CLASS_ID, 0, MAX_INSTANCE));

  if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
    Logger(LogLevel::ERROR) << "Failed to read the count of the parameters";
    logGeneralAndAdditionalStatus(response);
    return -1;
  }

  Buffer buffer(response.getData());
  CipUint paramsCount;
  buffer >> paramsCount;

  Logger(LogLevel::INFO) << "The device has " << paramsCount << "parameters";

  // Read Parameter Class Descriptor
  response = messageRouter.sendRequest(si
      , ServiceCodes::GET_ATTRIBUTE_SINGLE
      , EPath(ParameterObject::CLASS_ID, 0, CLASS_DESCRIPTOR));

  if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
    Logger(LogLevel::ERROR) << "Failed to read the class descriptor";
    logGeneralAndAdditionalStatus(response);
    return -1;
  }

  buffer = Buffer(response.getData());
  CipUint descriptor;
  buffer >> descriptor;

  Logger(LogLevel::INFO) << "Read the class descriptor=0x" << std::hex << (int)descriptor;
  bool allAttributes = descriptor & SUPPORTS_FULL_ATTRIBUTES;

  // Read and save parameters in a vector
  std::vector<ParameterObject> parameters;
  parameters.reserve(paramsCount);
  for (int i = 0; i < paramsCount; ++i) {
    parameters.emplace_back(i+1, allAttributes, si);
  }

  if (!parameters.empty()) {
    parameters[0].getType(); // Read type
    parameters[0].getActualValue<CipUint>(); // 2040
    parameters[0].getEngValue<CipUint>(); // 20.4
    parameters[0].getName();  // Freq
    parameters[0].getUnits();   // Hz
    // .. etc

    parameters[0].updateValue(si);
    parameters[0].getActualValue<CipUint>(); // updated value
  }

#if OS_Windows
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
