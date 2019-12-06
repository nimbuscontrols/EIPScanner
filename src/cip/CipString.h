//
// Created by Aleksey Timin on 12/4/19.
//

#ifndef EIPSCANNER_CIP_STRINGS_H
#define EIPSCANNER_CIP_STRINGS_H

#include <cstring>
#include <string>
#include <vector>
#include "Types.h"

namespace eipScanner {
namespace cip {
	template <typename T>
	class CipBaseString {
	public:
		CipBaseString() = default;
		explicit CipBaseString(const std::string& string) {
			_length = string.size();
			uint8_t buffer[_length];
			std::memcpy(buffer, string.data(), _length);

			_data = std::vector<uint8_t>(buffer, buffer + _length);
		}

		CipBaseString(const std::vector<uint8_t>& data) {
			_length = data.size();
			_data = data;
		}

		~CipBaseString() = default;

		std::string toStdString() const {
			char buffer[_length];
			std::memcpy(buffer, _data.data(), _length);

			return std::string(buffer, buffer + _length);
		}

		T getLength() const {
			return _length;
		}

		const std::vector<uint8_t> &getData() const {
			return _data;
		}

	private:
		T _length;
		std::vector<uint8_t> _data;
	};

	using CipShortString = CipBaseString<CipUsint>;
	using CipString = CipBaseString<CipUint>;
}
}

#endif  // EIPSCANNER_STRINGS_H
