#include "mio_queue.h"

namespace mio {

namespace internal {
    LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        queue *q = (queue*)GetWindowLong(hwnd, GWL_USERDATA);
        return q->queue_func(hwnd, message, wParam, lParam);
    }
} //namespace internal

queue::queue()
{
}

queue::queue( function_t queue_func ) : _callback(queue_func)
{
}

int queue::queue_func( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    return _callback(hwnd, message, wParam, lParam);
}

void queue::initialize()
{
    _msgWnd = CreateWindowEx(0, NULL, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
    SetWindowLong(_msgWnd, GWL_USERDATA, (LONG)this);
    SetWindowLong(_msgWnd, GWLP_WNDPROC, (LONG)internal::wndProc);
}

} //namespace mio
