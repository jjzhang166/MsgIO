#include "mio/mio.h"
#include "mio/unwindows.h"

#include "mio_loop.h"

namespace mio {
namespace  {
class connect_task {
public:
    typedef loop::connect_callback_t connect_callback_t;
#pragma warning(push)
#pragma warning(disable: 4200)
    struct pack {
        int        socket_family;
        int        socket_type;
        int        protocol;
        int        addrlen;
        int        timeout_msec;
        sockaddr   addr[0];
    };
#pragma warning(pop)

    connect_task(
            int socket_family, int socket_type, int protocol,
            const sockaddr* addr, int addrlen,
            double timeout_sec, connect_callback_t& callback) :
        m((pack*)::malloc(sizeof(pack)+addrlen)),
        m_callback(callback)
    {
        if(!m) { throw std::bad_alloc(); }
        m->socket_family = socket_family;
        m->socket_type   = socket_type;
        m->protocol      = protocol;
        m->addrlen       = addrlen;
        if (timeout_sec > 0) {
            m->timeout_msec = static_cast<int>(timeout_sec);
        } else {
            m->timeout_msec = -1;
        }
        ::memcpy(m->addr, addr, addrlen);
    }

    void operator() ()
    {
        int err = 0;
        int fd = ::socket(m->socket_family, m->socket_type, m->protocol);
        if(fd < 0) {
            err = WSAGetLastError();
            goto result;
        }

        WSAEVENT hEvent = WSACreateEvent();

        if (WSAEventSelect(fd, hEvent, FD_CONNECT) != NO_ERROR) {
            err = WSAGetLastError();
            goto error;
        }

        while (true) {
            if (connect(fd, m->addr, m->addrlen) >= 0) {
                goto result;
            }

            if(WSAGetLastError() != WSAEWOULDBLOCK) {
                goto error;
            }

            DWORD ret = WSAWaitForMultipleEvents(1, &hEvent, FALSE,m->timeout_msec, FALSE);
            if(ret == WSA_WAIT_EVENT_0) {
                goto result;
            }

            if(ret == WSA_WAIT_TIMEOUT) {
                goto error;
            }
        }

    error:
        err = WSAGetLastError();

        ::closesocket(fd);
        fd = -1;

    result:
        ::free(m);
        m_callback(fd, err);

        CloseHandle(hEvent);
    }

private:
    pack* m;
    connect_callback_t m_callback;

private:
    connect_task();
};

}  // noname namespace

void loop::connect( int socket_family, int socket_type, int protocol, const sockaddr* addr, int addrlen, double timeout_sec, connect_callback_t callback )
{
    connect_task t(socket_family, socket_type, protocol, addr, addrlen, timeout_sec, callback);
    submit(t);
}

} //namespace mio