#include "mio_loop_impl.h"

#define WAIT_TIME_OUT 1000

namespace mio {

namespace MESSAGE {
    enum {
        TASK = WM_USER + 10,
        IO_MSG
    };
}

namespace EVENT {
    enum {
        READ = FD_ACCEPT | FD_READ,
        WRITE = FD_WRITE
    };
}

loop_impl::loop_impl() : _end_flag(false)
{
    _hwnd = CreateWindowEx(0, TEXT("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
}

loop_impl::~loop_impl()
{
    ::DestroyWindow(_hwnd);
}

void loop_impl::start()
{
    if (!is_running()) {
        _thread.reset(new thread());
        _thread->run(bind(&loop_impl::thread_main, this));
    }
}

void loop_impl::run()
{
    start();
    join();
}

bool loop_impl::is_running() const
{
    return _thread.get() == NULL;
}

void loop_impl::end()
{
    _end_flag = true;
}

bool loop_impl::is_end() const
{
    return _end_flag;
}

void loop_impl::join()
{
    if (is_running()) {
        _thread->join();
    }
}

void loop_impl::thread_main()
{
    while(true) {
        if (_end_flag) { break; }

        DWORD ret = MsgWaitForMultipleObjects(0, NULL, FALSE, WAIT_TIME_OUT, QS_ALLEVENTS);
    }
}

void loop_impl::handle_message()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        switch (msg.message) {
        case MESSAGE::TASK:
            {
                std::auto_ptr<task_t> task(reinterpret_cast<task_t*>(msg.wParam));
                (*task)();
            }
            break;
        case MESSAGE::IO_MSG:
            {
                handle_io(msg);
            }
            break;
        default:
            break;
        }
    }
}


void loop_impl::handle_io( const MSG& msg )
{
    int socket = msg.wParam;
    int error = WSAGETSELECTERROR(msg.lParam);
    int e = WSAGETSELECTEVENT(msg.lParam);
    if (e & EVENT::READ) {
        event ev;
        (*_handlers[socket])(ev);
    }
}

void loop_impl::set_handler( shared_handler sh )
{
    thread_scoped_lock lock(_handlers_mutex);
    _handlers[sh->ident()] = sh;
}

void loop_impl::reset_handler( int fd )
{
    thread_scoped_lock lock(_handlers_mutex);
    _handlers.erase(fd);
}

void loop_impl::submit_impl( task_t f )
{
    std::auto_ptr<task_t> task(new task_t(f));
    PostMessage(_hwnd, MESSAGE::TASK, (WPARAM)task.get(), NULL);
    task.release();
}

shared_handler loop_impl::add_handler_impl( shared_handler sh )
{
    set_handler(sh);
    WSAAsyncSelect(sh->ident(), _hwnd, MESSAGE::IO_MSG, EVENT::READ );
    return sh;
}

void loop_impl::remove_handler( int ident )
{
    reset_handler(ident);
    WSAAsyncSelect(ident, _hwnd, NULL, NULL );
}

void loop_impl::connect( int socket_family, int socket_type, int protocol, const sockaddr* addr, int addrlen, double timeout_sec, connect_callback_t callback )
{
}

int loop_impl::listen( int socket_family, int socket_type, int protocol, const sockaddr* addr, int addrlen, listen_callback_t callback, int backlog /*= 1024*/ )
{
    return 0;
}

int loop_impl::add_timer( double value_sec, double interval_sec, function<bool ()> callback )
{
    return 0;
}

void loop_impl::remove_timer( int ident )
{

}

} //namespace mio