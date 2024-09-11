#ifndef EIPSCANNER_YASKAWA_MESSAGEROUTERREQUEST_H
#define EIPSCANNER_YASKAWA_MESSAGEROUTERREQUEST_H

#include <cstdint>
#include <vector>

#include "cip/Services.h"
#include "Yaskawa_EPath.h"

namespace eipScanner {
namespace cip {
	class Yaskawa_MessageRouterRequest {
	public:
		Yaskawa_MessageRouterRequest(CipUsint serviceCode, const Yaskawa_EPath& ePath, const std::vector<uint8_t> data);
		~Yaskawa_MessageRouterRequest();

		std::vector<uint8_t> pack() const;
	private:
		CipUsint _serviceCode;
		Yaskawa_EPath _ePath;
		std::vector<uint8_t> _data;
	};

}
}


#endif  // EIPSCANNER_YASKAWA_MESSAGEROUTERREQUEST_H
