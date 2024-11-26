//
// Created by Aleksey Timin on 12/9/19.
//

#ifndef EIPSCANNER_BASEOBJECT_H
#define EIPSCANNER_BASEOBJECT_H

#include "cip/Types.h"

namespace eipScanner {
	/**
	* @class BaseObject
	*
	* @brief Base class for all the CIP Objects
	*/
	class BaseObject {
	public:
		/**
		 * @brief Creates an CIP instance
		 * @param classId the class ID of the new instance
		 * @param instanceId the instance ID of the new instanc
		 */
		BaseObject(cip::CipUint classId, cip::CipUint instanceId);

		/**
		 * @brief Gets the class ID of the instance
		 * @return
		 */
		cip::CipUint getClassId() const;

		/**
		 * @brief Gets the instance ID of the instance
		 * @return
		 */
		cip::CipUint getInstanceId() const;

	private:
		cip::CipUint _classId;
		cip::CipUint _instanceId;
	};
}

#endif  // EIPSCANNER_BASEOBJECT_H
