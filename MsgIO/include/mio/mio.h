#pragma once

#include "functional.h"
#include "unwindows.h"
#include "memory.h"

namespace mio {

class basic_handler;
class handler;
class event;

typedef shared_ptr<basic_handler> shared_handler;
typedef weak_ptr<basic_handler> weak_handler;

class loop 
{
private: //Disable copy and assignment
	loop(const loop&);         
	void operator=(const loop&);

public:
    loop();
    ~loop();

    void start();

    void run();   // run = start + join

    bool is_running() const;

    void end();
    bool is_end() const;

    void join();

    void remove_handler(int fd);

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
    
    typedef void (*finalize_t)(void* user);

	void write(int fd, const void* buf, size_t size);

	void write(int fd, const void* buf, size_t size,
			finalize_t fin, void* user);

    template <typename Handler>
    shared_ptr<Handler> add_handler();
        template <typename Handler, typename A1>
    shared_ptr<Handler> add_handler(A1 a1);
        template <typename Handler, typename A1, typename A2>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2);
        template <typename Handler, typename A1, typename A2, typename A3>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);
        template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
    shared_ptr<Handler> add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);


	template <typename F>
    void submit(F f);
        template <typename F, typename A1>
    void submit(F f, A1 a1);
        template <typename F, typename A1, typename A2>
    void submit(F f, A1 a1, A2 a2);
        template <typename F, typename A1, typename A2, typename A3>
    void submit(F f, A1 a1, A2 a2, A3 a3);
        template <typename F, typename A1, typename A2, typename A3, typename A4>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);
        template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
    void submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

private:
    shared_handler add_handler_impl(shared_handler sh);

    typedef function<void ()> task_t;
    void submit_impl(task_t f);

private:
    void *_impl;
};

class event 
{
private: //Disable copy and assignment
    event(const event&);         
    void operator=(const event&);

public:
    event(){};
};

class basic_handler 
{
private: //Disable copy and assignment
    basic_handler(const basic_handler&);         
    void operator=(const basic_handler&);
    basic_handler();

public:
    typedef bool (*callback_t)(basic_handler*, event&);

    template <typename IMPL>
    basic_handler(int ident, IMPL* self) :
    m_ident(ident), m_callback(&static_callback<IMPL>) { }

    basic_handler(int ident, callback_t callback) :
    m_ident(ident), m_callback(callback) { }

    virtual ~basic_handler() { }

    int ident() const { return m_ident; }

    int fd() const { return ident(); }

    bool operator() (event& e);

private:
    int m_ident;

    callback_t m_callback;

private:
    template <typename IMPL>
    static bool static_callback(basic_handler* self, event& e)
    {
        return (*static_cast<IMPL*>(self))(e);
    }
};


class handler : public mio::enable_shared_from_this<handler>, public basic_handler {
public:
    handler(int fd) : basic_handler(fd, &callback_on_read) { }

    ~handler() { ::closesocket(fd()); }

    virtual void on_read(event& e) = 0;

public:
    template <typename IMPL>
    shared_ptr<IMPL> shared_self()
    {
        return static_pointer_cast<IMPL>(enable_shared_from_this<handler>::shared_from_this());
    }

    template <typename IMPL>
    shared_ptr<IMPL const> shared_self() const
    {
        return static_pointer_cast<IMPL>(enable_shared_from_this<handler>::shared_from_this());
    }

private:
    static inline bool callback_on_read(basic_handler* self, event& e)
    {
        static_cast<handler*>(self)->on_read(e);
        return true;
    }
    friend class basic_handler;
};


inline bool basic_handler::operator() (event& e)
{
    if(m_callback == handler::callback_on_read) {
        return handler::callback_on_read(this, e);
    } else {
        return m_callback(this, e);
    }
}

template <typename Handler>
shared_ptr<Handler> loop::add_handler()
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler())) ); }
template <typename Handler, typename A1>
shared_ptr<Handler> loop::add_handler(A1 a1)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1))) ); }
template <typename Handler, typename A1, typename A2>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2))) ); }
template <typename Handler, typename A1, typename A2, typename A3>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8, a9))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15))) ); }
template <typename Handler, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
shared_ptr<Handler> loop::add_handler(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
{ return static_pointer_cast<Handler>(add_handler_impl(
                                      shared_ptr<Handler>(new Handler(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16))) ); }

template <typename F>
inline void loop::submit(F f)
{ submit_impl(task_t(f)); }
template <typename F, typename A1>
inline void loop::submit(F f, A1 a1)
{ submit_impl(bind(f, a1)); }
template <typename F, typename A1, typename A2>
inline void loop::submit(F f, A1 a1, A2 a2)
{ submit_impl(bind(f, a1, a2)); }
template <typename F, typename A1, typename A2, typename A3>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3)
{ submit_impl(bind(f, a1, a2, a3)); }
template <typename F, typename A1, typename A2, typename A3, typename A4>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4)
{ submit_impl(bind(f, a1, a2, a3, a4)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{ submit_impl(bind(f, a1, a2, a3, a4, a5)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8, a9)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)); }
template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
inline void loop::submit(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
{ submit_impl(bind(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16)); }

} // namespace mio