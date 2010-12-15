#pragma once

#include "mio/mio.h"
#include "mio/thread.h"
#include "mio/sync.h"
#include "mio/unordered_map.h"

#include <memory>
#include <queue>

#include "mio_out.h"

#define IMPL (static_cast<loop_impl*>(_impl))
namespace mio {
namespace MESSAGE {
    enum MSG {
        IO_SOCKET = WM_USER + 10,
        IO_TIMER,
        EXIT
    };
}

namespace EVENT {
    enum EV {
        READ = FD_ACCEPT | FD_READ,
        WRITE = FD_WRITE
    };
}

class loop_impl
{
private: //Disable copy and assignment
    loop_impl(const loop_impl&);         
    void operator=(const loop_impl&);

public:
    typedef function<void ()> task_t;

public:
    loop_impl();
    ~loop_impl();

    void start(int num);

    void run(int num);   // run = start + join

    bool run_once(bool block);

    void add_thread(int num);

    bool is_running() const;

    void end();
    bool is_end() const;

    void join();

    void flush();

    void remove_handler(int ident);

    typedef function<void (int fd, int err)> connect_callback_t;

    void connect(
        int socket_family, int socket_type, int protocol,
        const sockaddr* addr, int addrlen,
        double timeout_sec, connect_callback_t callback);


    typedef function<void (int fd, int err)> listen_callback_t;

    int listen(
        int socket_family, int socket_type, int protocol,
        const sockaddr* addr, int addrlen,
        listen_callback_t callback,
        int backlog = 1024);

    bool isRead(int fd);

private:
    void set_handler(shared_handler sh);
    void reset_handler(int fd);

    void submit_impl(task_t f);
    void submit_impl(task_t *f);
    shared_handler add_handler_impl(shared_handler sh);

    void thread_main();
    void worker_main();
    bool handle_message(); // return true means get Exit message
    void handle_io_socket(WPARAM wParam, LPARAM lParam);
    void handle_io_timer(WPARAM wParam, LPARAM lParam);

private:
    HWND _hwnd;
    std::auto_ptr<out> _out;

    thread _thread;
    bool _is_running;

    typedef hash_map<int, shared_handler> map_container;
    typedef sync<map_container> concurrency_container;
    concurrency_container _handlers;

    typedef std::queue<shared_ptr<task_t>> task_queue;
    typedef sync<task_queue> concurrency_task_queue;
    concurrency_task_queue _task_queue;
    thread_semaphore _sem;

    typedef sync<std::vector<shared_ptr<thread>>> cocurrency_worker_queue;
    cocurrency_worker_queue _workers;

    friend class loop;
    friend class out;
};
} //namespace mio