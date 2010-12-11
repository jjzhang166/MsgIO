#include "mio_timer.h"
#include "mio.h"

namespace mio {
namespace {
static void timer_callback(HWND wnd, HANDLE flag, double value_sec, double internal_sec) 
{
    if (WaitForSingleObject(flag, static_cast<DWORD>(value_sec * 1000)) != WAIT_TIMEOUT) {
        return;
    }

    while(true) {
        PostMessage(wnd, MESSAGE::IO_TIMER, reinterpret_cast<WPARAM>(flag), NULL );
        if (WaitForSingleObject(flag, static_cast<DWORD>(internal_sec * 1000)) != WAIT_TIMEOUT) {
            return;
        }
    }
} 
} // noname namespace

kernel_timer::kernel_timer( HWND wnd, double value_sec, double internal_sec )
{
    _flag = CreateEvent(NULL, FALSE, FALSE, NULL);
    _thread.run(mio::bind(timer_callback, wnd, _flag, value_sec, internal_sec));
}

kernel_timer::~kernel_timer()
{
    SetEvent(_flag);
    CloseHandle(_flag);
}

int kernel_timer::timer_ident()
{
    return (int)_flag;
}

#pragma warning(push)
#pragma warning(disable:4355)
timer_handler::timer_handler( HWND hwnd, double value_sec, double internal_sec, callback_t callback ) :    kernel_timer(hwnd, value_sec, internal_sec), 
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
        shared_handler sh(new timer_handler(IMPL->_hwnd, value_sec, interval_sec, callback));
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