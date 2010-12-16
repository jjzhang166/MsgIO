#include "mio_loop.h"
#include "kernel_win32.h"

#include <cclog/cclog.h>

namespace mio {

loop_impl::loop_impl() : 
            _is_running(false)
{
    _thread.run(bind(&loop_impl::thread_main, this));
    while (_kernel.get() == NULL); //FIXME
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
    _kernel->end();
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
    _kernel.reset(new kernel());
    kernel::backlog backlog;
    LOG_TRACE("Loop Started");
    while(true) {
        int size = _kernel->wait(&backlog);
        if (size == -1) {
            break;
        }

        for (int i = 0;i<size;++i) {
            submit_impl(bind(&loop_impl::on_event, this, backlog[i]));
        }
    }
    LOG_TRACE("Loop ended");
}


void loop_impl::on_event( kernel::event e )
{
    concurrency_container::ref ref(_handlers);
    shared_handler handler = (*ref)[e.ident()];
    event ev;
    if (e.events() & EVKERNEL_READ) {
        if (!(*handler)(ev)) {
            reset_handler(e.ident());
        }
    } else {
        _out->on_write(e);
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

shared_handler loop_impl::add_handler_impl( shared_handler sh )
{
    set_handler(sh);
    _kernel->add_fd(sh->ident(), EVKERNEL_READ);
    return sh;
}

void loop_impl::remove_handler( int ident )
{
    reset_handler(ident);
    _kernel->remove_fd(ident, NULL);
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
        try
        {
            (*task)();
        }
        catch (std::exception &e)
        {
        	LOG_ERROR(e.what());
        }
    }
}

bool loop_impl::isRead( int fd )
{
    concurrency_container::auto_ref ref(_handlers);
    return ref->find(fd) != ref->end();
}

bool loop_impl::run_once( bool block )
{
    
    if (block)
        _sem.wait();
    else {
        if (!_sem.wait(0)) {
            return false;
        }
    }

    shared_ptr<task_t> task;
    {
        concurrency_task_queue::ref ref(_task_queue);
        task = ref->front();
        ref->pop();
    }
    if (task.get() == NULL) {
        throw system_error(-1, "you do run after ended!");
    }
    try
    {
        (*task)();
    }
    catch (std::exception &e)
    {
        LOG_ERROR(e.what());
    }
    return true;
}

void loop_impl::flush()
{
    while(run_once(false));
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

void loop::run_once() {
    IMPL->run_once(true);
}

void loop::run_nonblock() {
    IMPL->run_once(false);
}

bool loop::is_running() const {
    return IMPL->is_running();
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

void loop::flush()
{
    IMPL->flush();
}

void loop::join()
{
    IMPL->join();
}
} //namespace mio