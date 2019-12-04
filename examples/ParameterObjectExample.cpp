//
// Created by Aleksey Timin on 12/4/19.
//
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
int main() {
	Logger::setLogLevel(LogLevel::DEBUG);
	auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);
	auto messageRouter = std::make_shared<MessageRouter>();

	auto response = messageRouter->sendRequest(si
			, ServiceCodes::GET_ATTRIBUTE_SINGLE
			, EPath(ParameterObject::CLASS_ID, 0, MAX_INSTANCE)
			, {});

	if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
		Logger(LogLevel::ERROR) << "Failed to read the count of the parameters";
		logGeneralAndAdditionalStatus(response);
		return -1;
	}

	Buffer buffer(response.getData());
	CipUint paramsCount;
	buffer >> paramsCount;

	Logger(LogLevel::INFO) << "The device has " << paramsCount << "parameters";


	response = messageRouter->sendRequest(si
			, ServiceCodes::GET_ATTRIBUTE_SINGLE
			, EPath(ParameterObject::CLASS_ID, 0, CLASS_DESCRIPTOR)
			, {});

	if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
		Logger(LogLevel::ERROR) << "Failed to read the class descriptor";
		logGeneralAndAdditionalStatus(response);
		return -1;
	}

	buffer = Buffer(response.getData());
	CipUint descriptor;
	buffer >> descriptor;

	Logger(LogLevel::INFO) << "Read the class descriptor=0x" << std::hex << (int)descriptor;

	bool allAttributes = descriptor & 0x2;
	std::vector<ParameterObject> parameters;
	parameters.reserve(paramsCount);

	for (int i = 0; i < paramsCount; ++i) {
		parameters.emplace_back(i+1, allAttributes, si);
	}

}
