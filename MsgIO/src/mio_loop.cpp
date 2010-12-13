#include "mio_loop.h"

#include <cclog/cclog.h>

#define WAIT_TIME_OUT 1000

namespace mio {

loop_impl::loop_impl() : 
            _is_running(false),
            _start(CreateEvent(NULL, TRUE, FALSE, NULL)),
            _hwnd(0)
{
    _thread.run(bind(&loop_impl::thread_main, this));
    WaitForSingleObject(_start, INFINITE);
}

loop_impl::~loop_impl()
{
}

void loop_impl::start()
{
    if (!is_running()) {
        SetEvent(_start);
    }
}

void loop_impl::run()
{
    start();
    join();
}

bool loop_impl::is_running() const
{
    return _is_running;
}

void loop_impl::end()
{
    _is_running = false;
}

bool loop_impl::is_end() const
{
    return !_is_running;
}

void loop_impl::join()
{
    _thread.join();
}

void loop_impl::thread_main()
{
    _hwnd = CreateWindowEx(0, TEXT("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
    SetEvent(_start);

    LOG_TRACE("Loop start");
    WaitForSingleObject(_start, INFINITE);
    _is_running = true;

    while(true) {
        if (!_is_running) { break; }

        DWORD ret = MsgWaitForMultipleObjects(0, NULL, FALSE, WAIT_TIME_OUT, QS_ALLEVENTS);
        if (ret != WAIT_TIMEOUT) {
            handle_message();
        }
    }

    LOG_TRACE("Loop ended");
    ::DestroyWindow(_hwnd);
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
        case MESSAGE::IO_SOCKET:
            {
                handle_io_socket(msg);
            }
            break;
        case MESSAGE::IO_TIMER:
            {
                handle_io_timer(msg);
            }
            break;
        default:
            break;
        }
    }
}


void loop_impl::handle_io_socket( const MSG& msg )
{
    int socket = msg.wParam;
    int error = WSAGETSELECTERROR(msg.lParam);
    int e = WSAGETSELECTEVENT(msg.lParam);
    if (e & EVENT::READ) {
        event ev;
        (*(*_handlers.lock())[socket])(ev);
        //(*_handlers[socket])(ev);
    }
}


void loop_impl::handle_io_timer( const MSG&msg )
{
    int ident = static_cast<int>(msg.wParam);
    event ev;
    //if (!(*_handlers[ident])(ev)) {
    if (!(*(*_handlers.lock())[ident])(ev)) {
        reset_handler(ident);
    }
}

void loop_impl::set_handler( shared_handler sh )
{
    concurrency_container::auto_ref ref = _handlers.lock();
    if (ref->find(sh->ident()) != ref->end()) {
        throw std::logic_error("HANDLE ALREADY exist!");
        return;
    }

}

void loop_impl::reset_handler( int fd )
{
    _handlers.lock()->erase(fd);
}

void loop_impl::submit_impl( task_t f )
{
    std::auto_ptr<task_t> task(new task_t(f));
    PostMessage(_hwnd, MESSAGE::TASK, reinterpret_cast<WPARAM>(task.get()), NULL);
    task.release();
}

shared_handler loop_impl::add_handler_impl( shared_handler sh )
{
    set_handler(sh);
    WSAAsyncSelect(sh->ident(), _hwnd, MESSAGE::IO_SOCKET, EVENT::READ );
    return sh;
}

void loop_impl::remove_handler( int ident )
{
    reset_handler(ident);
    WSAAsyncSelect(ident, _hwnd, NULL, NULL );
}

} //namespace mio