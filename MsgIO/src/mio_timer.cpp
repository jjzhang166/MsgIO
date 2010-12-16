#include "mio_timer.h"
#include "mio/mio.h"

namespace mio {
kernel_timer::kernel_timer( kernel& kern, double value_sec, double interval_sec )
{
    kern.add_timer(&_timer, value_sec, interval_sec);
}

kernel_timer::~kernel_timer()
{
}

int kernel_timer::timer_ident()
{
    return _timer.ident();
}

#pragma warning(push)
#pragma warning(disable:4355)
timer_handler::timer_handler( kernel & kern, double value_sec, double internal_sec, callback_t callback ) :    kernel_timer(kern, value_sec, internal_sec), 
        basic_handler(timer_ident(), this ),
        _callback(callback)
{

}
#pragma warning(pop)

bool timer_handler::operator()( event& e )
{
    return _callback();
}

int loop::add_timer(double value_sec, double interval_sec,
              function<bool ()> callback)
{
    if (value_sec > 0.0 && interval_sec > 0.0) {
        shared_handler sh(new timer_handler(*IMPL->_kernel.get(), value_sec, interval_sec, callback));
        IMPL->set_handler(sh);
        return sh->ident();
    }

    return -1;
}

void loop::remove_timer(int ident)
{
    IMPL->reset_handler(ident);
}

} //namespace mio