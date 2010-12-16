#include "kernel_win32.h"

namespace mio {
namespace MESSAGE {
    enum MSG {
        IO_SOCKET = WM_USER + 10,
        IO_TIMER,
        EXIT
    };
}
namespace  {
    inline uint64_t make_u64(int hi, int lo) {
        return ((uint64_t)lo) | ((uint64_t)hi << 32);
    }
} //noname namespace

kernel::kernel()
{
    _wnd = CreateWindowEx(0, TEXT("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
}

kernel::~kernel()
{
    ::DestroyWindow(_wnd);
}

int kernel::add_fd( int fd, short event )
{
    return ::WSAAsyncSelect(fd, _wnd, MESSAGE::IO_SOCKET, event);
}

int kernel::remove_fd( int fd, short event )
{
    return ::WSAAsyncSelect(fd, _wnd, NULL,NULL);
}

int kernel::ident() const
{
    return reinterpret_cast<int>(_wnd);
}

int kernel::wait( backlog* result )
{
    return wait(result, INFINITE);
}

int kernel::wait( backlog* result, int timeout_msec )
{
    std::vector<uint64_t> &vec = result->_ev;
    vec.clear();

    DWORD ret = MsgWaitForMultipleObjects(0, NULL, FALSE, timeout_msec, QS_ALLEVENTS);
    if (ret == WAIT_TIMEOUT)
        return 0;

    MSG msg;
    while (PeekMessage(&msg, _wnd, 0, 0, PM_REMOVE)) {
        switch (msg.message) {
        case MESSAGE::IO_SOCKET:
            {
                vec.push_back(make_u64(WSAGETSELECTEVENT(msg.lParam), msg.wParam));
            }
            break;
        case MESSAGE::IO_TIMER:
            {
                vec.push_back(make_u64(msg.lParam, msg.wParam));
            }
            break;
        case MESSAGE::EXIT:
            {
                return -1;
            }
            break;
        default:
            break;
        }
    }

    return vec.size();
}

namespace {
static void timer_callback(HWND wnd, HANDLE flag, double value_sec, double internal_sec) 
{
    if (WaitForSingleObject(flag, static_cast<DWORD>(value_sec * 1000)) != WAIT_TIMEOUT) {
        return;
    }

    while(true) {
        PostMessage(wnd, MESSAGE::IO_TIMER, reinterpret_cast<WPARAM>(flag), EVKERNEL_READ );
        if (WaitForSingleObject(flag, static_cast<DWORD>(internal_sec * 1000)) != WAIT_TIMEOUT) {
            return;
        }
    }
} 
} // noname namespace

kernel::timer::timer() : _flag(0) 
{
    _flag = CreateEvent(NULL, FALSE, FALSE, NULL);
}

kernel::timer::~timer()
{
    stop();
}

int kernel::timer::ident()
{
    return reinterpret_cast<int>(_flag);
}

void kernel::timer::stop()
{
    if (_flag) {
        SetEvent(_flag);
        CloseHandle(_flag);
        _flag = 0;
    }
}

int kernel::add_timer( timer* tm, double value_sec, double internal_sec )
{
    tm->_flag = CreateEvent(NULL, FALSE, FALSE, NULL);
    tm->_thread.run(mio::bind(timer_callback, _wnd, tm->_flag, value_sec, internal_sec));
    return tm->ident();
}

int kernel::remove_timer( timer *tm )
{
    tm->stop();
    return 0;
}

int kernel::remove( event e )
{
    return remove_fd(e.ident(), NULL);
}

void kernel::end()
{
    PostMessage(_wnd, MESSAGE::EXIT, NULL, NULL);
}

kernel::event kernel::backlog::operator[]( int n ) const
{
    return event(_ev[n]);
}

} //namespace mio