//
// Created by Aleksey Timin on 11/18/19.
//

#include <utils/Logger.h>
#include "IOConnection.h"
#include "utils/Logger.h"

namespace eipScanner {
	using utils::Logger;
	using utils::LogLevel;

	IOConnection::IOConnection()
		: _o2tNetworkConnectionId{0}
		, _t2oNetworkConnectionId{0}
		, _o2tAPI{0}
		, _t2oAPI{0}
		, _o2t_timer{0}
		, _t2o_timer{0}
		, _connectionTimeoutMultiplier{0}
		, _connectionTimeoutCount{0}
		, _o2tSequenceNumber{0}
		, _t2oSequenceNumber{0}
		, _serialNumber{0}
		, _outputData()
		, _receiveDataHandle([](auto data){})
		, _closeHandle([](){}) {
	}

	IOConnection::~IOConnection() = default;

	void IOConnection::setDataToSend(const std::vector<uint8_t> &data) {

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
		_connectionTimeoutCount += period.count() * 1000;
		if (_connectionTimeoutCount > _connectionTimeoutMultiplier * _t2oAPI) {
			Logger(LogLevel::WARNING) << "Connection SeriaNumber=" << _serialNumber << " is closed by timeout";
			_closeHandle();
			return false;
		}

		return true;
	}
}
