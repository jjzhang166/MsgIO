#pragma once

#include "mio_loop.h"
#include "mio_thread.h"

#include <memory>
#include <hash_map>

namespace mio {

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

    void start();

    void run();   // run = start + join

    bool is_running() const;

    void end();
    bool is_end() const;

    void join();

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

    int add_timer(double value_sec, double interval_sec,
        function<bool ()> callback);

    void remove_timer(int ident);

private:
    void set_handler(shared_handler sh);
    void reset_handler(int fd);

    void submit_impl(task_t f);
    shared_handler add_handler_impl(shared_handler sh);

    void thread_main();
    void handle_message();
    void handle_io(const MSG& msg);

private:
    bool _end_flag;
    HWND _hwnd;
    std::auto_ptr<thread> _thread;
    stdext::hash_map<int, shared_handler> _handlers;
    thread_mutex _handlers_mutex;

    friend class loop;
};
} //namespace mio