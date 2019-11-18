//
// Created by flipback on 11/18/19.
//

#include "IOConnection.h"

namespace eipScanner {
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
		, _inputData()
		, _outputData() {
	}

	IOConnection::~IOConnection() = default;

	void IOConnection::setDataToSend(const std::vector<uint8_t> &data) {

	}

	void IOConnection::setReceiveDataListener(eipScanner::IOConnection::ReceiveDataHandle handle) {

	}

	void IOConnection::setCloseListener(eipScanner::IOConnection::CloseHandle handle) {

	}
}
