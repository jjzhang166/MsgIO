#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#undef max
#undef min

//some type defines
typedef unsigned __int64 uint64_t;

struct iovec { //WSABUF capitable
    size_t iov_len;
    void *iov_base;
};