#include "mio/mio.h"
#include "mio/unwindows.h"
#include "mio_loop.h"

namespace mio {
namespace  {
class listen_handler : public handler {
public:
	typedef loop::listen_callback_t listen_callback_t;

	listen_handler(int fd, HWND hwnd, listen_callback_t callback) :
		handler(fd), _hwnd(hwnd), _callback(callback) {}

	~listen_handler() {}

	void on_read(event& e)
	{
		//while(true) {
			int err = 0;
			int sock = ::accept(fd(), NULL, NULL);
            WSAAsyncSelect(sock, _hwnd, NULL, NULL);
			if(sock == INVALID_SOCKET) {
                err = WSAGetLastError();
                if(err == WSAEWOULDBLOCK || err == WSAEINTR) {
					return;
				}

				_callback(sock, err);

				throw system_error(err, "accept failed");
			}

			try {
				_callback(sock, err);
			} catch (...) {
				::closesocket(sock);
			}
		//}
	}

private:
	listen_callback_t _callback;
    HWND _hwnd;

private:
	listen_handler();
	listen_handler(const listen_handler&);
};
} //namespace noname

int loop::listen(
                 int socket_family, int socket_type, int protocol,
                 const sockaddr* addr, int addrlen,
                 listen_callback_t callback,
                 int backlog)
{
    int lsock = ::socket(socket_family, socket_type, protocol);
    if(lsock < 0) {
        throw system_error(WSAGetLastError(), "socket() failed");
    }

    int on = 1;
    if(::setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) < 0) {
        ::closesocket(lsock);
        throw system_error(WSAGetLastError(), "setsockopt failed");
    }

    if(::bind(lsock, addr, addrlen) < 0) {
        ::closesocket(lsock);
        throw system_error(WSAGetLastError(), "bind failed");
    }

    if(::listen(lsock, backlog) < 0) {
        ::closesocket(lsock);
        throw system_error(WSAGetLastError(), "listen failed");
    }

    try {
        add_handler<listen_handler>(lsock, IMPL->_hwnd, callback);
    } catch (...) {
        ::closesocket(lsock);
        throw;
    }

    return lsock;
}

} //namespace mio