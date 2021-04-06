//
// Created by Stefan Broekman on 02/04/21.
//

#ifndef EIPSCANNER_SOCKETS_PLATFORM_H
#define EIPSCANNER_SOCKETS_PLATFORM_H

#include <system_error>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
namespace detail {
  class win32_error_category : public std::error_category {
    public:
      char const* name() const noexcept override final;
      std::string message(int c) const override final;
  };
}
extern detail::win32_error_category const& win32_error_category();

#define EIPSCANNER_SOCKET_ERROR(err) WSA##err
#else
#define EIPSCANNER_SOCKET_ERROR(err) err
#endif

#endif // EIPSCANNER_SOCKETS_PLATFORM_H
