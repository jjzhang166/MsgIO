#pragma once

#include "mio_loop.h"
#include "kernel_win32.h"

namespace mio {
class kernel_timer {
public:
    kernel_timer(kernel& kern, double value_sec, double interval_sec);
    ~kernel_timer();

protected:
    int timer_ident();

private:
    kernel::timer _timer;
};

class timer_handler : public kernel_timer, public basic_handler
{
private:
    typedef function<bool ()> callback_t;

public:
    timer_handler(kernel &kern, double value_sec, double internal_sec, callback_t callback);
    bool operator() (event& e);

private:
     callback_t _callback;
};
} //namespace mio