//
// Created by Vincent Prince on 05/22/20.
//

#include "src/cip/connectionManager/NetworkConnectionParametersBuilder.h"

#include <utility>
#include <assert.h>
#include "src/utils/Buffer.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	using utils::Buffer;

	NetworkConnectionParametersBuilder::NetworkConnectionParametersBuilder(CipUdint val, bool lfo) :
     _value{val}, _lfo{lfo} {

	}

	NetworkConnectionParametersBuilder NetworkConnectionParametersBuilder::setRedundantOwner(RedundantOwner val) {
		if (_lfo) {
			_value |= (val << 31);
		} else {
			_value |= (val << 15);
		}
		return *this;
	}

	NetworkConnectionParametersBuilder& NetworkConnectionParametersBuilder::setConnectionType(ConnectionType val) {
		if (_lfo) {
			_value |= (val << 29);
		} else {
			_value |= (val << 13);
		}
		return *this;
	}

	NetworkConnectionParametersBuilder& NetworkConnectionParametersBuilder::setPriority(Priority val) {
		if (_lfo) {
			_value |= (val << 26);
		}else {
			_value |= (val << 10);
		}
		return *this;
	}

	NetworkConnectionParametersBuilder& NetworkConnectionParametersBuilder::setType(Type val) {
		if (_lfo) {
			_value |= (val << 25);
		} else {
			_value |= (val << 9);
		}
		return *this;
	}

	NetworkConnectionParametersBuilder& NetworkConnectionParametersBuilder::setConnectionSize(CipUint val) {
		CipUdint mask = 0x000001FF;
		if (_lfo) {
			mask = 0x0000FFFF;
		}
		_value |= val & mask;
		return *this;
	}

	CipUdint NetworkConnectionParametersBuilder::build() {
		return _value;
	}

	NetworkConnectionParametersBuilder::RedundantOwner NetworkConnectionParametersBuilder::getRedundantOwner() const {
		if (_lfo) {
			return static_cast<NetworkConnectionParametersBuilder::RedundantOwner>(((_value & (1 << 31)) >> 31));
		} else {
			return static_cast<NetworkConnectionParametersBuilder::RedundantOwner>(((_value & (1 << 15)) >> 15));
		}
	}

	NetworkConnectionParametersBuilder::ConnectionType NetworkConnectionParametersBuilder::getConnectionType() const {
		if (_lfo) {
			return static_cast<NetworkConnectionParametersBuilder::ConnectionType>(((_value & (3 << 29)) >> 29));
		} else {
			return static_cast<NetworkConnectionParametersBuilder::ConnectionType>(((_value & (3 << 13)) >> 13));
		}
	}

	NetworkConnectionParametersBuilder::Priority NetworkConnectionParametersBuilder::getPriority() const {
		if (_lfo) {
			return static_cast<NetworkConnectionParametersBuilder::Priority>(((_value & (3 << 26)) >> 26));
		} else {
			return static_cast<NetworkConnectionParametersBuilder::Priority>(((_value & (3 << 10)) >> 10));
		}
	}

	NetworkConnectionParametersBuilder::Type NetworkConnectionParametersBuilder::getType() const {
		if (_lfo) {
			return static_cast<NetworkConnectionParametersBuilder::Type>(((_value & (1 << 25)) >> 25));
		} else {
			return static_cast<NetworkConnectionParametersBuilder::Type>(((_value & (1 << 9)) >> 9));
		}
	}

	CipUint NetworkConnectionParametersBuilder::getConnectionSize() const {
		CipUdint mask = 0x000001FF;
		if (_lfo) {
			mask = 0x0000FFFF;
		}
		return static_cast<CipUint>(_value & mask);
}

}
}
}
