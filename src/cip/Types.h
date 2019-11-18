//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_CIP_TYPES_H
#define EIPSCANNER_CIP_TYPES_H

#include <cstdint>

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


	enum class CipDataTypes : CipUsint {
		ANY = 0x00, /**< data type that can not be directly encoded */
		BOOL = 0xC1, /**< boolean data type */
		SINT = 0xC2, /**< 8-bit signed integer */
		INT = 0xC3, /**< 16-bit signed integer */
		DINT = 0xC4, /**< 32-bit signed integer */
		LINT = 0xC5, /**< 64-bit signed integer */
		USINT = 0xC6, /**< 8-bit unsigned integer */
		UINT = 0xC7, /**< 16-bit unsigned integer */
		UDINT = 0xC8, /**< 32-bit unsigned integer */
		ULINT = 0xC9, /**< 64-bit unsigned integer */
		REAL = 0xCA, /**< Single precision floating point */
		LREAL = 0xCB, /**< Double precision floating point*/
		STIME = 0xCC, /**< Synchronous time information*, type of DINT */
		DATE = 0xCD, /**< Date only*/
		DATE_OF_DAY = 0xCE, /**< Time of day */
		DATE_AND_TIME = 0xCF, /**< Date and time of day */
		STRING = 0xD0, /**< Character string, 1 byte per character */
		BYTE = 0xD1, /**< 8-bit bit string */
		WORD = 0xD2, /**< 16-bit bit string */
		DWORD = 0xD3, /**< 32-bit bit string */
		LWORD = 0xD4, /**< 64-bit bit string */
		STRING2 = 0xD5, /**< Character string, 2 byte per character */
		FTIME = 0xD6, /**< Duration in micro-seconds, high resolution; range of DINT */
		LTIME = 0xD7, /**< Duration in micro-seconds, high resolution, range of LINT */
		ITIME = 0xD8, /**< Duration in milli-seconds, short; range of INT*/
		STRINGN = 0xD9, /**< Character string, N byte per character */
		SHORT_STRING = 0xDA, /**< Character string, 1 byte per character, 1 byte
                             length indicator */
		TIME = 0xDB, /**< Duration in milli-seconds; range of DINT */
		EPATH = 0xDC, /**< CIP path segments*/
		ENG_UNIT = 0xDD, /**< Engineering Units*/
		/* definition of some CIP structs */
		/* need to be validated in IEC 61131-3 subclause 2.3.3 */
		USINT_USINT = 0xA0, /**< Used for CIP Identity attribute 4 Revision*/
		USINT6 = 0xA2, /**< Struct for MAC Address (six USINTs)*/
		MEMBER_LIST = 0xA3, /**< */
		BYTE_ARRAY = 0xA4, /**< */
	};
}
}
#endif //EIPSCANNER_CIP_TYPES_H
