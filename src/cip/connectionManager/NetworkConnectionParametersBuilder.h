//
// Created by Vincent Prince on 05/22/20.
//

#ifndef EIPSCANNER_CIP_CONNECTIONMANAGER_NETWORKCONNECTIONPARAMETERSBUILDER_H
#define EIPSCANNER_CIP_CONNECTIONMANAGER_NETWORKCONNECTIONPARAMETERSBUILDER_H

#include "cip/Types.h"

namespace eipScanner {
namespace cip {
namespace connectionManager {

	class NetworkConnectionParametersBuilder {
	public:
		enum RedundantOwner {
			EXCLUSIVE,
			REDUNDANT,
		};

		enum ConnectionType {
			NULL_TYPE,
			MULTICAST,
			P2P,
			RESERVED,
		};

		enum Priority {
			LOW_PRIORITY,
			HIGH_PRIORITY,
			SCHEDULED,
			URGENT,
		};

		enum Type {
			FIXED,
			VARIABLE,
		};


		NetworkConnectionParametersBuilder(CipUdint val = 0, bool lfo = false);
		virtual ~NetworkConnectionParametersBuilder() = default;

		NetworkConnectionParametersBuilder setRedundantOwner(RedundantOwner val);
		NetworkConnectionParametersBuilder& setConnectionType(ConnectionType val);
		NetworkConnectionParametersBuilder& setPriority(Priority val);
		NetworkConnectionParametersBuilder& setType(Type val);
		NetworkConnectionParametersBuilder& setConnectionSize(CipUint val);
		CipUdint build();

		RedundantOwner getRedundantOwner() const;
		ConnectionType getConnectionType() const;
		Priority getPriority() const;
		Type getType() const;
		CipUint getConnectionSize() const;

	private:
		CipUdint _value;
		bool _lfo;
	};
}
}
}
#endif //EIPSCANNER_CIP_CONNECTIONMANAGER_NETWORKCONNECTIONPARAMETERSBUILDER_H
