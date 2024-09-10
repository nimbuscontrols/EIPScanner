//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_GENERALSTATUSCODES_H
#define EIPSCANNER_CIP_GENERALSTATUSCODES_H

#include "src/cip/Types.h"

namespace eipScanner {
namespace cip {

	enum GeneralStatusCodes : CipUsint {
		//! Service was successfully performed.
		SUCCESS = 0x00,

		//! A connection related service failed along the connection path.
		CONNECTION_FAILURE = 0x01,

		//! Resources needed for the object to perform the  requested service
		//! were unavailable.
		RESOURCE_UNAVAILABLE = 0x02,

		//! See CIPStatusCodes.InvalidParameter, which is the preferred value
		//! to use for this condition.
		INVALID_PARAMETER_VALUE = 0x03,

		//! The path segment identifier or the segment syntax was not understood
		//! by the processing node. Path processing shall stop when a path segment
		//! error is encountered.
		PATH_SEGMENT_ERROR = 0x04,

		//! The path is referencing an object class, instance, or structure element
		//! that is not known or is not contained in the processing node. Path
		//! processing shall stop when a path destination unknown error is encountered.
		PATH_DESTINATION_UNKNOWN = 0x05,

		//! Only part of the expected data was transferred.
		PARTIAL_TRANSFER = 0x06,

		//! The messaging connection was lost.
		CONNECTION_LOST = 0x07,

		//! The requested service was not implemented or was not defined for this
		//! object Class/Instance.
		SERVICE_NOT_SUPPORTED = 0x08,

		//! Invalid attribute data detected.
		INVALID_ATTRIBUTE_VALUE = 0x09,

		//! An attribute in the Get_Attribute_List or Set_Attribute_List response
		//! has a non-zero status.
		ATTRIBUTE_LIST_ERROR = 0x0A,

		//! The object is already in the mode/state being requested by the service.
		ALREADY_IN_REQUESTED_MODE_OR_STATE = 0x0B,

		//! The object cannot perform the requested service in its current state/mode.
		OBJECT_STATE_CONFLICT = 0x0C,

		//! The requested instance of object to be created already exists.
		OBJECT_ALREADY_EXISTS = 0x0D,

		//! A request to modify a non-modifiable attribute was received.
		ATTRIBUTE_NOT_SETTABLE = 0x0E,

		//! A permission/privilege check failed.
		PRIVILEGE_VIOLATION = 0x0F,

		//! The device's current mode/state prohibits the execution of the requested
		//! service.
		DEVICE_STATE_CONFLICT = 0x10,

		//! The data to be transmitted in the response buffer is larger than the
		//! allocated response buffer.
		REPLY_DATA_TOO_LARGE = 0x11,

		//! The service specified an operation that is going to fragment a primitive
		//! data value, i.e. half a REAL data type.
		FRAGMENTATION_OF_PRIMITIVE_VALUE = 0x12,

		//! The service did not supply enough data to perform the requested operation.
		NOT_ENOUGH_DATA = 0x13,

		//! The attribute specified in the request is not supported.
		ATTRIBUTE_NOT_SUPPORTED = 0x14,

		//! The service was supplied with more data than was expected.
		TOO_MUCH_DATA = 0x15,

		//! The object specified does not exist on the device.
		OBJECT_DOES_NOT_EXIST = 0x16,

		//! The fragmentation sequence for this service is not currently active for
		//! this data.
		SVCFRAG_SEQNC_NOT_IN_PROGRESS = 0x17,

		//! The attribute data of this object was not saved prior to the requested
		//! service.
		NO_STORED_ATTRIBUTE_DATA = 0x18,

		//! The attribute data of this object was not saved due to a failure following
		//! the attempt.
		STORE_OPERATION_FAILURE = 0x19,

		//! The service request packet was too large for transmission on a network
		//! in the path to the destination. The routing device was forced to abort the
		//! service.
		ROUTING_FAILURE_REQUEST_SIZE = 0x1A,

		//! The service response packet was too large for transmission on a network in the
		//! path from the destination. The routing device was forced to abort the service.
		ROUTING_FAILURE_RESPONSE_SIZE = 0x1B,

		//! The service did not supply an attribute in a list of attributes that was needed by
		//! the service to perform the requested behavior.
		MISSING_ATTRIBUTE_LIST_ENTRY = 0x1C,

		//! The service is returning the list of attributes supplied with status information
		//! for those attributes that were invalid.
		INVALID_ATTRIBUTE_LIST = 0x1D,

		//! An embedded service resulted in an error.
		EMBEDDED_SERVICE_ERROR = 0x1E,

		//! A vendor specific error has been encountered. The Additional Code Field of
		//! the Error Response defines the particular error encountered. Use of this
		//! General Error Code should only be performed when none of the Error Codes
		//! presented in this table or within an Object Class definition accurately reflect
		//! the error.
		VENDOR_SPECIFIC = 0x1F,

		//! A parameter associated with the request was invalid. This code is used when a
		//! parameter does not meet the requirements of this specification and/or the
		//! requirements defined in an Application Object Specification.
		INVALID_PARAMETER = 0x20,

		//! An attempt was made to write to a write-once medium (e.g. WORM drive,
		//! PROM) that has already been written, or to modify a value that cannot be
		//! changed once established.
		WRITE_ONCE_WRITTEN = 0x21,

		//! An invalid reply is received (e.g. reply service code does not match the request
		//! service code, or reply message is shorter than the minimum expected reply
		//! size). This status code can serve for other causes of invalid replies.
		INVALID_REPLY_RECEIVED = 0x22,

		//! The Key Segment that was included as the first segment in the path does not
		//! match the destination module. The object specific status shall indicate which
		//! part of the key check failed.
		KEY_FAILURE_IN_PATH = 0x25,

		//! The size of the path which was sent with the Service Request is either not large
		//! enough to allow the Request to be routed to an object or too much routing data
		//! was included.
		PATH_SIZE_INVALID = 0x26,

		//! An attempt was made to set an attribute that is not able to be set at this time.
		UNEXPECTED_ATTRIBUTE = 0x27,

		//! The Member ID specified in the request does not exist in the specified
		//! Class/Instance/Attribute
		INVALID_MEMBER_ID = 0x28,

		//! A request to modify a non-modifiable member was received
		MEMBER_NOT_SETTABLE = 0x29
	};

}
}
#endif //EIPSCANNER_CIP_GENERALSTATUSCODES_H
