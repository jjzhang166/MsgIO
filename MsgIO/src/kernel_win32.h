#pragma once
#include "mio/unwindows.h"
#include "mio/thread.h"
#include <vector>

namespace mio {
static const short EVKERNEL_READ = FD_ACCEPT | FD_READ;
static const short EVKERNEL_WRITE = FD_WRITE;

class kernel {
private: //Disable copy and assignment
    kernel(const kernel&);         
    void operator=(const kernel&);

public:
    kernel();
    ~kernel();

    class event 
    {
    public:
        event() { }
        explicit event(uint64_t data) : _data(data) { }
        ~event() { }

        int ident() const { return _data & 0xffffffff; }
        int events() const { return _data >> 32; }

    private:
        uint64_t _data;

        uint64_t data()   const { return _data; }
    };


    int add_fd(int fd, short event);
    int remove_fd(int fd, short event);


    class timer 
    {
    private: //Disable copy and assignment
        timer(const timer&);         
        void operator=(const timer&);

    public:
        timer();
        ~timer();
        int ident();

    private:
        void stop();

    private:
        thread _thread;
        HANDLE _flag;

        friend class kernel;
    };

    int add_timer(timer* tm, double value_sec, double internal_sec);
    int remove_timer(timer *tm);
    
    int ident() const;


    class backlog {
    private: //Disable copy and assignment
        backlog(const backlog&);         
        void operator=(const backlog&);

    public:
        backlog() {}
        ~backlog() {}
        event operator[] (int n) const;

    private:
        std::vector<uint64_t> _ev;
        friend class kernel;
    };

    int wait(backlog* result);
    int wait(backlog* result, int timeout_msec);
    void end();

    int reactivate(event e);
    int remove(event e); //can only used for socket

private: //members
    HWND _wnd;
};
} //namespace mio