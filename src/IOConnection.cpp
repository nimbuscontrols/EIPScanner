//
// Created by Aleksey Timin on 11/18/19.
//

#include "IOConnection.h"
#include "eip/CommonPacketItemFactory.h"
#include "eip/CommonPacket.h"
#include "cip/connectionManager/NetworkConnectionParams.h"
#include "utils/Buffer.h"
#include "utils/Logger.h"

namespace eipScanner {
	using utils::Logger;
	using utils::LogLevel;
	using utils::Buffer;
	using eip::CommonPacket;
	using eip::CommonPacketItemFactory;
	using cip::connectionManager::NetworkConnectionParams;

	IOConnection::IOConnection()
			: _o2tNetworkConnectionId{0}
			, _t2oNetworkConnectionId{0}
			, _o2tAPI{0}
			, _t2oAPI{0}
			, _o2tTimer{0}
			, _t2o_timer{0}
			, _connectionTimeoutMultiplier{0}
			, _connectionTimeoutCount{0}
			, _o2tSequenceNumber{0}
			, _transportTypeTrigger{0}
			, _o2tRealTimeFormat{0}
			, _t2oRealTimeFormat{0}
			, _t2oSequenceNumber{0}
			, _sequenceValueCount{0}
			, _serialNumber{0}
			, _outputData()
			, _receiveDataHandle([](auto a, auto b, auto data) {})
			, _closeHandle([]() {}) {
	}

	IOConnection::~IOConnection() = default;

	void IOConnection::setDataToSend(const std::vector<uint8_t> &data) {
		_sequenceValueCount++;
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
		Buffer buffer(data);
		cip::CipUdint runtimeHeader = 0;
		cip::CipUint sequenceValueCount = 0;

		if (_t2oRealTimeFormat) {
			buffer >> runtimeHeader;
		}

		if ((_transportTypeTrigger & NetworkConnectionParams::CLASS1) > 0
			|| (_transportTypeTrigger & NetworkConnectionParams::CLASS3) > 0) {
			buffer >> sequenceValueCount;
		}


		_receiveDataHandle(runtimeHeader, sequenceValueCount,
				std::vector<uint8_t>(data.begin() + buffer.pos(), data.end()));
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

			Buffer buffer;
			if (_o2tRealTimeFormat) {
				cip::CipUdint header = 1; //TODO: Always RUN
				buffer << header;
			}

			if ((_transportTypeTrigger & NetworkConnectionParams::CLASS1) > 0
				|| (_transportTypeTrigger & NetworkConnectionParams::CLASS3) > 0) {
				buffer << _sequenceValueCount;
			}

			buffer << _outputData;
			commonPacket << factory.createConnectedDataItem(buffer.data());


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
