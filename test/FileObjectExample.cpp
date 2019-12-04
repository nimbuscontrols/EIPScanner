//
// Created by Aleksey Timin on 11/24/19.
//

#include <fstream>
#include "FileObject.h"
#include "utils/Logger.h"
#include "fileObject/FileObjectState.h"

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;
using eipScanner::FileObject;

int main() {
	Logger::setLogLevel(LogLevel::DEBUG);
	auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);
	auto messageRouter = std::make_shared<MessageRouter>();

	FileObject edsFile(0xC8, si, messageRouter);
	edsFile.beginUpload(si, [](GeneralStatusCodes status, const std::vector<uint8_t> &fileContent) {
		if (status == GeneralStatusCodes::SUCCESS) {
			std::ofstream outFile("Device.eds", std::ios::out | std::ios::trunc);
			std::string content(fileContent.begin(), fileContent.end());
			outFile << content;
		}
	});

	edsFile.getState()->SyncState(si);
	while (edsFile.handleTransfers(si)) {
		continue;
	};
}