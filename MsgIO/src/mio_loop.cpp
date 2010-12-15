#include "mio_loop.h"

#include <cclog/cclog.h>

#define WAIT_TIME_OUT 1000

namespace mio {

loop_impl::loop_impl() : 
            _is_running(false),
            _hwnd(0)
{
    _thread.run(bind(&loop_impl::thread_main, this));
    while(_hwnd == 0); //FIXME TODO(wsxiaoys)
    _out.reset(new out(this));
}

loop_impl::~loop_impl()
{
    end();
    join();
}

void loop_impl::start( int num )
{
    if (!is_running()) {
        _is_running = true;
    }

    add_thread(num);
}

void loop_impl::run( int num )
{
    start(num);
    join();
}


void loop_impl::add_thread( int num )
{

    cocurrency_worker_queue::ref ref(_workers);
    for (int i = 0;i<num;++i) {
        ref->push_back(shared_ptr<thread>(new thread()));

        try
        {
            ref->back()->run(bind(&loop_impl::worker_main, this));
        }
        catch (...)
        {
            ref->pop_back();
        }
    }
}

bool loop_impl::is_running() const
{
    return _is_running;
}

void loop_impl::end()
{
    _is_running = false;
    PostMessage(_hwnd, MESSAGE::EXIT, NULL, NULL);
    for (int i = 0;i<(int)_workers.unsafe_ref().size();++i) {
        submit_impl(NULL);
    }
}

bool loop_impl::is_end() const
{
    return !_is_running;
}

void loop_impl::join()
{
    _thread.join();
    for (int i = 0;i<(int)_workers.unsafe_ref().size();++i) {
        _workers.unsafe_ref()[i]->join();
    }
}

void loop_impl::thread_main()
{
    _hwnd = CreateWindowEx(0, TEXT("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

    while(true) {
        DWORD ret = MsgWaitForMultipleObjects(0, NULL, FALSE, WAIT_TIME_OUT, QS_ALLEVENTS);
        if (ret != WAIT_TIMEOUT) {
            if (handle_message()) {
                break;
            }
        }
    }

    LOG_TRACE("Loop ended");
    ::DestroyWindow(_hwnd);
    _hwnd = 0;
}

bool loop_impl::handle_message()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        switch (msg.message) {
        case MESSAGE::IO_SOCKET:
            {
                submit_impl(bind(&loop_impl::handle_io_socket, this, msg.wParam, msg.lParam));
            }
            break;
        case MESSAGE::IO_TIMER:
            {
                submit_impl(bind(&loop_impl::handle_io_timer, this, msg.wParam, msg.lParam));
            }
            break;
        case MESSAGE::EXIT:
            {
                return true;
            }
            break;
        default:
            break;
        }
    }
    return false;
}


void loop_impl::handle_io_socket( WPARAM wParam, LPARAM lParam )
{
    int socket = wParam;
    int error = WSAGETSELECTERROR(lParam);
    int e = WSAGETSELECTEVENT(lParam);
    if (e & EVENT::READ) {
        event ev;
        (*(*_handlers.lock())[socket])(ev);
    }

    if (e & EVENT::WRITE) {
        _out->on_write(socket);
    }
}


void loop_impl::handle_io_timer( WPARAM wParam, LPARAM lParam )
{
    int ident = static_cast<int>(wParam);
    event ev;
    if (!(*(*_handlers.lock())[ident])(ev)) {
        reset_handler(ident);
    }
}

void loop_impl::set_handler( shared_handler sh )
{
    concurrency_container::ref ref(_handlers);
    if (ref->find(sh->ident()) != ref->end()) {
        throw std::logic_error("HANDLE ALREADY exist!");
        return;
    }

    (*ref)[sh->ident()] = sh;
}

void loop_impl::reset_handler( int fd )
{
    _handlers.lock()->erase(fd);
}

void loop_impl::submit_impl( task_t f )
{
    std::auto_ptr<task_t> task(new task_t(f));
    _task_queue.lock()->push(shared_ptr<task_t>(task.get()));
    _sem.signal();
    task.release();
}

void loop_impl::submit_impl( task_t *f )
{
    _task_queue.lock()->push(shared_ptr<task_t>(f));
    _sem.signal();
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

void loop_impl::worker_main()
{
    while(true){
        _sem.wait();

        shared_ptr<task_t> task;
        {
            concurrency_task_queue::ref ref(_task_queue);
            task = ref->front();
            ref->pop();
        }
        if (task.get() == NULL) {
            return;
        }
        (*task)();
    }
}

bool loop_impl::isRead( int fd )
{
    concurrency_container::auto_ref ref(_handlers);
    return ref->find(fd) != ref->end();
}

loop::loop()
{
    _impl = new loop_impl();
}

loop::~loop()
{
    delete IMPL;
}

void loop::start(int num)
{
    IMPL->start(num);
}

void loop::run(int num)
{
    IMPL->run(num);
}

mio::shared_handler loop::add_handler_impl( shared_handler sh )
{
    return IMPL->add_handler_impl(sh);
}

void loop::remove_handler( int fd )
{
    return IMPL->remove_handler(fd);
}

void loop::submit_impl( task_t f )
{
    IMPL->submit_impl(f);
}

void loop::end()
{
    IMPL->end();
}

bool loop::is_end() const
{
    return IMPL->is_end();
}

void loop::join()
{
    IMPL->join();
}
} //namespace mio