//
// Created by Stefan Broekman on 02/04/21.
//
// Only include in local (source) files related to (Win)Sockets, to prevent unintentional overrides in
// unwanted places. I.e. errno is not supported on Windows for WinSock only, it is for e.g. file I/O.
//

#ifndef EIPSCANNER_SOCKETS_PLATFORM_H
#define EIPSCANNER_SOCKETS_PLATFORM_H

#include <cerrno>

#if defined (__unix__)
#define SOCKET_ERRNO() (errno)
#define SOCKET_SHUTDOWN_OPERATION (SHUT_RDWR)
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#define SOCKET_ERRNO() (WSAGetLastError())
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
