//
// Created by Aleksey Timin on 12/9/19.
//

#ifndef EIPSCANNER_MOCKS_H
#define EIPSCANNER_MOCKS_H

#include <gmock/gmock.h>
#include "EIPScanner/MessageRouter.h"

class TMockMessageRouter : public eipScanner::MessageRouter {
public:
	using SPtr = std::shared_ptr<TMockMessageRouter>;

	MOCK_CONST_METHOD3(sendRequest, eipScanner::cip::MessageRouterResponse(
			eipScanner::SessionInfoIf::SPtr si,
			eipScanner::cip::CipUsint service,
			const eipScanner::cip::EPath &path));

	MOCK_CONST_METHOD4(sendRequest, eipScanner::cip::MessageRouterResponse(
			eipScanner::SessionInfoIf::SPtr si,
			eipScanner::cip::CipUsint service,
			const eipScanner::cip::EPath &path,
			const std::vector<uint8_t> &data));

	MOCK_CONST_METHOD5(sendRequest, eipScanner::cip::MessageRouterResponse(
			eipScanner::SessionInfoIf::SPtr si,
			eipScanner::cip::CipUsint service,
			const eipScanner::cip::EPath &path,
			const std::vector<uint8_t> &data,
			const eipScanner::eip::CommonPacketItem::Vec&));
};

class TMockSessionInfo : public eipScanner::SessionInfoIf {
public:
	using SPtr = std::shared_ptr<TMockSessionInfo>;
	MOCK_CONST_METHOD1(sendAndReceive, eipScanner::eip::EncapsPacket(const eipScanner::eip::EncapsPacket& packet));
	MOCK_CONST_METHOD0(getSessionHandle, eipScanner::cip::CipUdint());
	MOCK_CONST_METHOD0(getRemoteEndPoint, eipScanner::sockets::EndPoint());
};

class TMockSocket : public eipScanner::sockets::BaseSocket {
public:
	using SPtr = std::shared_ptr<TMockSocket>;
	TMockSocket() : eipScanner::sockets::BaseSocket("", 0) {}
	MOCK_CONST_METHOD1(Send, void(const std::vector<uint8_t>& data));
	MOCK_CONST_METHOD1(Receive, std::vector<uint8_t>(size_t size));
};

#endif //EIPSCANNER_MOCKS_H
