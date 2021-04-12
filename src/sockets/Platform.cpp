//
// Created by Stefan Broekman on 02/04/21.
//

#include "Platform.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <windows.h>

namespace eipScanner {
namespace sockets {
  char const* win32ErrorCategory::name() const noexcept {
    return "Win32Error";
  }

  std::string win32ErrorCategory::message(int c) const {
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

  /*static*/win32ErrorCategory const& win32ErrorCategory::category() {
    static win32ErrorCategory c;
    return c;
  }
}
}
#endif
