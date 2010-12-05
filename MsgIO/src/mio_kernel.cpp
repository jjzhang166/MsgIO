#include "mio_kernel.h"

namespace mio {

kernel::kernel()
{
    _msgWnd = CreateWindowEx(0, TEXT("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
}

kernel::~kernel()
{
    DestroyWindow(_msgWnd);
}

HWND kernel::ident()
{
    return _msgWnd;
}

} //namespace mio