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

#ifdef UNDER_CE
typedef
int
(WSAAPI * LPFN_WSAASYNCSELECT)(
                               IN SOCKET s,
                               IN HWND hWnd,
                               IN u_int wMsg,
                               IN long lEvent
                               );
#define HWND_MESSAGE NULL
extern LPFN_WSAASYNCSELECT WSAAsyncSelect;
#endif

#define MESSAGE_CLASS TEXT("Message")

//init method
namespace mio {
    void init();
}