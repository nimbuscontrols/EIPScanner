//
// Created by flipback on 11/18/19.
//

#ifndef EIPSCANNER_IOCONNECTION_H
#define EIPSCANNER_IOCONNECTION_H

#include <memory>
#include <vector>
#include <functional>
#include "cip/Types.h"
#include "sockets/UDPSocket.h"

namespace eipScanner {
	class ConnectionManager;

	class IOConnection {
		friend class ConnectionManager;
	public:
		using ReceiveDataHandle = std::function<void(const std::vector<uint8_t>&, const IOConnection&)>;
		using CloseHandle = std::function<void(const IOConnection&)>;

		using WPtr=std::weak_ptr<IOConnection>;
		using SPtr=std::shared_ptr<IOConnection>;

		~IOConnection();

		void setDataToSend(const std::vector<uint8_t>& data);
		void setReceiveDataListener(ReceiveDataHandle handle);
		void setCloseListener(CloseHandle handle);

	private:
		IOConnection();

		cip::CipUdint _o2tNetworkConnectionId;
		cip::CipUdint _t2oNetworkConnectionId;
		cip::CipUdint _o2tAPI;
		cip::CipUdint _t2oAPI;

		cip::CipUdint _o2t_timer;
		cip::CipUdint _t2o_timer;

		cip::CipUsint _connectionTimeoutMultiplier;
		cip::CipUsint _connectionTimeoutCount;

		cip::CipUdint _o2tSequenceNumber;
		cip::CipUdint _t2oSequenceNumber;

		std::vector<uint8_t> _inputData;
		std::vector<uint8_t> _outputData;

		sockets::UDPSocket::WPtr _socket;
	};
}

#endif  // EIPSCANNER_IOCONNECTION_H
