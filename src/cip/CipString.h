//
// Created by Aleksey Timin on 12/4/19.
//

#ifndef EIPSCANNER_CIP_STRINGS_H
#define EIPSCANNER_CIP_STRINGS_H

#include <cstring>
#include <iterator>
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
			std::copy(string.begin(), string.end(), std::back_inserter(_data));
		}

		CipBaseString(const std::vector<uint8_t>& data) {
			_length = data.size();
			_data = data;
		}

		~CipBaseString() = default;

		std::string toStdString() const {
			return std::string(_data.begin(), _data.end());
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
