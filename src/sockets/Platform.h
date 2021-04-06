//
// Created by Stefan Broekman on 02/04/21.
//
// Only include in local (source) files related to (Win)Sockets, to prevent unintentional overrides in
// unwanted places. I.e. errno is not supported on Windows for WinSock only, it is for e.g. file I/O.
//

#ifndef EIPSCANNER_SOCKETS_PLATFORM_H
#define EIPSCANNER_SOCKETS_PLATFORM_H

#include <cerrno>
#include <system_error>

#if defined (__unix__)
#define SOCKET_ERRNO() (errno)
#define SOCKET_ERROR_CATEGORY() (std::generic_category())
#define SOCKET_CLOSE(socketFd) (close(socketFd))
#define SOCKET_SHUTDOWN_OPERATION (SHUT_RDWR)

#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <windows.h>

namespace detail
{
  class win32_error_category : public std::error_category {
    public:
      char const* name() const noexcept override final;
      std::string message(int c) const override final;
  };
}
extern detail::win32_error_category const& win32_error_category();

#define SOCKET_ERRNO() (WSAGetLastError())
#define SOCKET_ERROR_CATEGORY() (win32_error_category())
#define SOCKET_CLOSE(socketFd) (closesocket(socketFd))
#define SOCKET_SHUTDOWN_OPERATION (SD_BOTH)

//Used socket error codes - redefine to the respective WSA error code.
#undef EINTR
#define EINTR (WSAEINTR)
#undef EINPROGRESS
#define EINPROGRESS (WSAEINPROGRESS)
#undef ETIMEDOUT
#define ETIMEDOUT (WSAETIMEDOUT)
#endif

#endif // EIPSCANNER_SOCKETS_PLATFORM_H
