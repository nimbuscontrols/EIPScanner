//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_TYPES_H
#define EIPSCANNER_CIP_TYPES_H

namespace eipScanner {
namespace cip {
	typedef uint8_t CipOctet; /**< 8 bit value that indicates particular data type */
	typedef uint8_t CipBool; /**< Boolean data type */
	typedef uint8_t CipByte; /**< 8-bit bit string */
	typedef uint16_t CipWord; /**< 16-bit bit string */
	typedef uint32_t CipDword; /**< 32-bit bit string */
	typedef uint8_t CipUsint; /**< 8-bit unsigned integer */
	typedef uint16_t CipUint; /**< CipUint 16-bit unsigned integer */
	typedef uint32_t CipUdint; /**< CipUdint 32-bit unsigned integer */
	typedef int8_t CipSint; /**< 8-bit signed integer */
	typedef int16_t CipInt; /**< 16-bit signed integer */
	typedef int32_t CipDint; /**< 32-bit signed integer */
	typedef float CipReal; /**< 32-bit IEEE 754 floating point */
	typedef double CipLreal; /**< 64-bit IEEE 754 floating point */
}
}

#endif //EIPSCANNER_CIP_TYPES_H
