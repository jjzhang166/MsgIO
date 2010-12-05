#include "mio_queue.h"

namespace mio {

queue::queue()
{
    _msgWnd = CreateWindowEx(0, TEXT("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
}

queue::~queue()
{
    DestroyWindow(_msgWnd);
}

HWND queue::getHandle()
{
    return _msgWnd;
}

} //namespace mio