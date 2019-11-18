//
// Created by flipback on 11/18/19.
//

#ifndef EIPSCANNER_IOCONNECTION_H
#define EIPSCANNER_IOCONNECTION_H

#include <memory>

namespace eipScanner {
	class IOConnection {
	public:
		using WPtr=std::weak_ptr<IOConnection>;
		using SPtr=std::shared_ptr<IOConnection>;
	};
}

#endif  // EIPSCANNER_IOCONNECTION_H
