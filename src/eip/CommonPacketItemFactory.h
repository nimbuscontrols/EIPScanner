//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_EIP_COMMONPACKETITEMFACTORY_H
#define EIPSCANNER_EIP_COMMONPACKETITEMFACTORY_H

#include <vector>
#include "CommonPacketItem.h"

namespace eipScanner {
namespace eip {
	class CommonPacketItemFactory {
	public:
		CommonPacketItem createNullAddressItem() const;

		CommonPacketItem createUnconnectedDataItem(const std::vector<uint8_t> &data) const;
	};
}
}
#endif  // EIPSCANNER_EIP_COMMONPACKETITEMFACTORY_H
