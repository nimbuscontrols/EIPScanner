//
// Created by Stefan Broekman on 02/04/21.
//

#include "Platform.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)

namespace detail
{
  char const* win32_error_category::name() const noexcept {
    return "Win32Error";
  }

  std::string win32_error_category::message(int c) const {
    char error[UINT8_MAX];
    auto len = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, static_cast<DWORD>(c), 0, error, sizeof(error), nullptr);
    if (len == 0) {
      return "N/A";
    }
    // trim trailing newline
    while (len && (error[len - 1] == '\r' || error[len - 1] == '\n')) {
      --len;
    }
    return std::string(error, len);
  }
}

detail::win32_error_category const& win32_error_category() {
  static detail::win32_error_category c;
  return c;
}
#endif
