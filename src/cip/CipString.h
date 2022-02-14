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

			if (!string.empty()) {
				_data.reserve(string.size());
				std::memcpy(&_data.front(), &string.front(), _length);
			}
		}

		CipBaseString(const std::vector<uint8_t>& data) {
			_length = data.size();
			_data = data;
		}

		~CipBaseString() = default;

		std::string toStdString() const {
			std::string string;

			if (!_data.empty()) {
				string.reserve(_data.size());
				std::memcpy(&string.front(), &_data.front(), _length);
			}

			return string;
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
