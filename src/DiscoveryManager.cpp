//
// Created by Aleksey Timin on 12/17/19.
//
#include <system_error>

#include "src/eip/EncapsPacketFactory.h"
#include "src/eip/CommonPacket.h"
#include "src/sockets/UDPSocket.h"
#include "src/utils/Logger.h"
#include "src/utils/Buffer.h"

#include "src/DiscoveryManager.h"

namespace eipScanner {
	using namespace cip;

	using eip::EncapsPacketFactory;
	using eip::EncapsPacket;
	using eip::EncapsStatusCodes;
	using eip::EncapsCommands;
	using eip::CommonPacket;

	using sockets::EndPoint;
	using sockets::UDPSocket;
	using utils::LogLevel;
	using utils::Logger;
	using utils::Buffer;

	DiscoveryManager::DiscoveryManager(const std::string& broadCastAddress, int port,
			std::chrono::milliseconds receiveTimout)
		: _broadCastAddress(broadCastAddress, port)
		, _receiveTimout(receiveTimout) {
	}

	DiscoveryManager::~DiscoveryManager() = default;

	IdentityItem::Vec DiscoveryManager::discover() const {
		eipScanner::IdentityItem::Vec devices;

		auto socket = makeSocket();
		socket->Send(EncapsPacketFactory().createListIdentityPacket().pack());

		try {
			for(;;) {
				const size_t EIP_MAX_PACKET_SIZE = 504;
				auto data = socket->Receive(EIP_MAX_PACKET_SIZE);

				CommonPacket commonPacket;
				commonPacket.expand(std::vector<uint8_t>(data.begin()+EncapsPacket::HEADER_SIZE, data.end()));

				for (int i=0; i < commonPacket.getItems().size(); ++i) {
					Buffer buffer(commonPacket.getItems()[i].getData());
					CipUint ignore;
					sockets::EndPoint socketAddr("", 0);

					buffer >> ignore >> socketAddr;

					CipUint vendorId, deviceType, productCode;
					CipRevision revision;
					CipWord status;
					CipUdint serialNumber;
					CipShortString productName;

					buffer >> vendorId >> deviceType >> productCode
						   >> revision >> status
						   >> serialNumber >> productName;

					IdentityObject identityObject(i);
					identityObject.setVendorId(vendorId);
					identityObject.setDeviceType(deviceType);
					identityObject.setProductCode(productCode);
					identityObject.setRevision(revision);
					identityObject.setStatus(status);
					identityObject.setSerialNumber(serialNumber);
					identityObject.setProductName(productName.toStdString());

					devices.push_back(IdentityItem{.identityObject=identityObject, .socketAddress=socketAddr});
				}
			}
		} catch (std::system_error& er) {
			if (er.code().value() != DISCOVERY_SOCKET_RECEIVE_END_ERROR_CODE) {
				throw er;
			}
		}

		return devices;
	}

	sockets::BaseSocket::SPtr DiscoveryManager::makeSocket() const {
		auto socket =  std::make_shared<UDPSocket>(_broadCastAddress);
		socket->setRecvTimeout(_receiveTimout);

		int broadcast = 1;
		if(setsockopt(socket->getSocketFd(), SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) < 0) {
			throw std::system_error(sockets::BaseSocket::getLastError(), sockets::BaseSocket::getErrorCategory());
		}

		return socket;
	}
}
