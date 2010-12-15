#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#undef max
#undef min
#undef ERROR

//some type defines
typedef unsigned __int64 uint64_t;

#ifndef MSGPACK_VREFBUFFER_H__
struct iovec { //WSABUF capitable
    size_t iov_len;
    void *iov_base;
};
#endif