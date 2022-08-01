//
// Created by Aleksey Timin on 11/16/19.
//

#include <cassert>

#include "eip/EncapsPacketFactory.h"
#include "utils/Buffer.h"
#include "MessageRouter.h"
#include "cip/MessageRouterRequest.h"
#include "cip/MessageRouterResponse.h"
#include "eip/CommonPacketItemFactory.h"
#include "eip/CommonPacket.h"
#include "utils/Buffer.h"
#include "utils/Logger.h"

namespace eipScanner {
	using namespace cip;
	using namespace utils;
	using eip::CommonPacketItemFactory;
	using eip::CommonPacket;
	using eip::CommonPacketItem;
	using eip::EncapsPacket;
	using eip::EncapsPacketFactory;

	MessageRouter::MessageRouter(bool use_8_bit_path_segments) 
    : _use_8_bit_path_segments(use_8_bit_path_segments)
    {}

	MessageRouter::~MessageRouter() = default;

	MessageRouterResponse
	MessageRouter::sendRequest(SessionInfoIf::SPtr si, CipUsint service, const EPath &path,
							   const std::vector<uint8_t> &data) const {
		return this->sendRequest(si, service, path, data, {});
	}

	MessageRouterResponse
	MessageRouter::sendRequest(SessionInfoIf::SPtr si, CipUsint service, const EPath &path,
							   const std::vector<uint8_t> &data,
							   const std::vector<eip::CommonPacketItem>& additionalPacketItems) const {
		assert(si);

		Logger(LogLevel::INFO) << "Send request: service=0x" << std::hex << static_cast<int>(service)
							   << " epath=" << path.toString();

		MessageRouterRequest request{service, path, data, _use_8_bit_path_segments};

		CommonPacketItemFactory commonPacketItemFactory;
		CommonPacket commonPacket;
		commonPacket << commonPacketItemFactory.createNullAddressItem();
		commonPacket << commonPacketItemFactory.createUnconnectedDataItem(request.pack());

		for(auto& item : additionalPacketItems) {
			commonPacket << item;
		}

		auto packetToSend = EncapsPacketFactory()
				.createSendRRDataPacket(si->getSessionHandle(), 0, commonPacket.pack());

		auto receivedPacket = si->sendAndReceive(packetToSend);

		Buffer buffer(receivedPacket.getData());
		cip::CipUdint interfaceHandle = 0;
		cip::CipUint timeout = 0;
		std::vector<uint8_t> receivedData(receivedPacket.getData().size() - 6);

		buffer >> interfaceHandle >> timeout >> receivedData;
		commonPacket.expand(receivedData);

		MessageRouterResponse response;
		const CommonPacketItem::Vec &items = commonPacket.getItems();

		response.expand(items.at(1).getData());
		if (items.size() > 2) {
			response.setAdditionalPacketItems(
					CommonPacketItem::Vec(items.begin() + 2, items.end()));
		}


		return response;
	}

	MessageRouterResponse
	MessageRouter::sendRequest(SessionInfoIf::SPtr si, CipUsint service, const EPath &path) const {
		return this->sendRequest(si, service, path, {}, {});
	}
}
