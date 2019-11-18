//
// Created by Aleksey Timin on 11/18/19.
//

#include <utils/Logger.h>
#include "IOConnection.h"
#include "utils/Logger.h"
#include "eip/CommonPacketItemFactory.h"
#include "eip/CommonPacket.h"

namespace eipScanner {
	using utils::Logger;
	using utils::LogLevel;
	using eip::CommonPacket;
	using eip::CommonPacketItemFactory;

	IOConnection::IOConnection()
			: _o2tNetworkConnectionId{0}, _t2oNetworkConnectionId{0}, _o2tAPI{0}, _t2oAPI{0}, _o2tTimer{0},
			  _t2o_timer{0}, _connectionTimeoutMultiplier{0}, _connectionTimeoutCount{0}, _o2tSequenceNumber{0},
			  _t2oSequenceNumber{0}, _serialNumber{0}, _outputData(), _receiveDataHandle([](auto data) {}),
			  _closeHandle([]() {}) {
	}

	IOConnection::~IOConnection() = default;

	void IOConnection::setDataToSend(const std::vector<uint8_t> &data) {
		_outputData = data;
	}

	void IOConnection::setReceiveDataListener(ReceiveDataHandle handle) {
		_receiveDataHandle = std::move(handle);
	}

	void IOConnection::setCloseListener(CloseHandle handle) {
		_closeHandle = std::move(handle);
	}

	void IOConnection::notifyReceiveData(const std::vector<uint8_t> &data) {
		_connectionTimeoutCount = 0;
		_receiveDataHandle(data);
	}

	bool IOConnection::notifyTick(std::chrono::milliseconds period) {
		auto periodInMicroS = period.count() * 1000;
		_connectionTimeoutCount += periodInMicroS;
		if (_connectionTimeoutCount > _connectionTimeoutMultiplier * _t2oAPI) {
			Logger(LogLevel::WARNING) << "Connection SeriaNumber=" << _serialNumber << " is closed by timeout";
			_closeHandle();
			return false;
		}

		_o2tTimer += periodInMicroS;
		if (_o2tTimer >= _o2tAPI) {
			_o2tSequenceNumber++;
			CommonPacket commonPacket;
			CommonPacketItemFactory factory;
			commonPacket << factory.createSequenceAddressItem(_o2tNetworkConnectionId, _o2tSequenceNumber);
			commonPacket << factory.createConnectedDataItem(_outputData);


			auto ptr = _socket.lock();
			if (ptr) {
				ptr->Send(commonPacket.pack());
				_o2tTimer = 0;
			} else {
				Logger(LogLevel::WARNING) << "Connection SeriaNumber="
										  << _serialNumber << " has no socket. Close.";
				return false;
			}
		}

		return true;
	}
}
