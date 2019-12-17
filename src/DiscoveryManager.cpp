//
// Created by Aleksey Timin on 12/17/19.
//
#include <system_error>
#include <cerrno>

#include "eip/EncapsPacketFactory.h"
#include "sockets/UDPSocket.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

#include "DiscoveryManager.h"

namespace eipScanner {
	using namespace cip;

	using eip::EncapsPacketFactory;
	using eip::EncapsPacket;
	using eip::EncapsStatusCodes;
	using eip::EncapsCommands;

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
				auto data = socket->Receive(504);
				Buffer buffer(data);
				std::vector<uint8_t> header(EncapsPacket::HEADER_SIZE);
				buffer >> header;

				CipUint count;
				buffer >> count;
				for (int i = 1; i <= count; ++i) {
					CipUint ignore;
					buffer >> ignore >> ignore >> ignore;
					sockets::EndPoint socketAddr("", 0);

					buffer >> socketAddr;

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
			if (er.code().value() != EAGAIN) {
				throw er;
			}
		}

		return devices;
	}

	sockets::BaseSocket::UPtr DiscoveryManager::makeSocket() const {
		auto socket =  std::make_unique<UDPSocket>(_broadCastAddress);
		socket->setRecvTimeout(_receiveTimout);

		int broadcast = 1;
		if(setsockopt(socket->getSocketFd(), SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
			throw std::system_error(errno, std::generic_category());
		}

		return socket;
	}
}
