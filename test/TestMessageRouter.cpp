//
// Created by Aleksey Timin on 12/10/19.
//

#include <gtest/gtest.h>
#include "Mocks.h"
#include "MessageRouter.h"
#include "eip/CommonPacket.h"
#include "eip/CommonPacketItem.h"
#include "eip/CommonPacketItemFactory.h"
#include "eip/EncapsPacketFactory.h"
#include "cip/MessageRouterRequest.h"
#include "cip/MessageRouterResponse.h"

using namespace eipScanner;

TEST(TestMessageRouter, ShouldFormAndParseCommonPackets) {
	auto mockSession = std::make_shared<TMockSessionInfo>();
	MessageRouter router;
	auto service = cip::ServiceCodes::SET_ATTRIBUTE_SINGLE;
	cip::EPath path(1,2,3);
	std::vector<uint8_t> data = {0,1,2,3};

	cip::MessageRouterRequest request(service, path, data);
	eip::CommonPacketItemFactory commonPacketItemFactory;

	auto additionalPacketItem = commonPacketItemFactory.createNullAddressItem();

	eip::CommonPacket sendCommonPacket;
	sendCommonPacket << commonPacketItemFactory.createNullAddressItem();
	sendCommonPacket << commonPacketItemFactory.createUnconnectedDataItem(request.pack());
	sendCommonPacket << additionalPacketItem;

	auto packetToSend = eip::EncapsPacketFactory()
			.createSendRRDataPacket(10, 0, sendCommonPacket.pack());

	eip::CommonPacket receiveCommonPacket;
	receiveCommonPacket << commonPacketItemFactory.createNullAddressItem();
	receiveCommonPacket << commonPacketItemFactory.createUnconnectedDataItem({
		static_cast<cip::CipUsint>(service | 0x80),
		0,
		cip::GeneralStatusCodes::SUCCESS,
		0
	});
	receiveCommonPacket << additionalPacketItem;

	auto receivedPacket = eip::EncapsPacketFactory()
			.createSendRRDataPacket(10, 0, receiveCommonPacket.pack());

	EXPECT_CALL(*mockSession, getSessionHandle()).WillOnce(::testing::Return(10));
	EXPECT_CALL(*mockSession, sendAndReceive(packetToSend)).WillOnce(::testing::Return(receivedPacket));

	auto response = router.sendRequest(mockSession, service,
			path,
			data,
			{additionalPacketItem});

	EXPECT_EQ(cip::GeneralStatusCodes::SUCCESS, response.getGeneralStatusCode());
	EXPECT_EQ(1, response.getAdditionalPacketItems().size());
	EXPECT_EQ(additionalPacketItem, response.getAdditionalPacketItems().at(0));
}