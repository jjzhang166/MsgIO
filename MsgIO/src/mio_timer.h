#pragma once

#include "mio_loop.h"
namespace mio {
class kernel_timer {
public:
    kernel_timer(HWND wnd, double value_sec, double internal_sec);
    ~kernel_timer();

protected:
    int timer_ident();

private:
    thread _thread;
    HANDLE _flag;
};

class timer_handler : public kernel_timer, public basic_handler
{
private:
    typedef function<bool ()> callback_t;

public:
    timer_handler(HWND hwnd, double value_sec, double internal_sec, callback_t callback);
    bool operator() (event& e);

private:
     callback_t _callback;
};
} //namespace mio