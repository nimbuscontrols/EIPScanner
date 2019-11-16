//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_EIP_ENCAPSPACKETFACTORY_H
#define EIPSCANNER_EIP_ENCAPSPACKETFACTORY_H

#include "EncapsPacket.h"
#include "cip/Types.h"

namespace eipScanner {
namespace eip {
	class EncapsPacketFactory {
	public:
		EncapsPacket createRegisterSessionPacket() const;
		EncapsPacket createUnRegisterSessionPacket(cip::CipUdint sessionHandle) const;
		EncapsPacket createSendRRDataPacket(cip::CipUdint sessionHandle,
				cip::CipUint timeout, std::vector<uint8_t> data) const;
	};
}
}



#endif  // EIPSCANNER_ENCAPSPACKETFACTORY_H
