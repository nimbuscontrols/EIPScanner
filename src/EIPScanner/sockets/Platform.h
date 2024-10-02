//
// Created by Stefan Broekman on 02/04/21.
//

#ifndef EIPSCANNER_SOCKETS_PLATFORM_H
#define EIPSCANNER_SOCKETS_PLATFORM_H

#include <system_error>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)

#define EIPSCANNER_SOCKET_ERROR(err) WSA##err

namespace eipScanner {
namespace sockets {
  class win32ErrorCategory : public std::error_category {
    public:
      char const* name() const noexcept override final;
      std::string message(int c) const override final;

      static win32ErrorCategory const& category();
  };
}
}

#else
#define EIPSCANNER_SOCKET_ERROR(err) err
#endif

#endif // EIPSCANNER_SOCKETS_PLATFORM_H
