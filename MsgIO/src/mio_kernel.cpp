#include "mio_kernel.h"
#include "unwindows.h"

#define KERNEL_IO_MSG (WM_USER + 0x1)

namespace mio {

kernel::kernel()
{
    _fd = (int)CreateWindowEx(0, TEXT("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
}

kernel::~kernel()
{
    DestroyWindow((HWND)_fd);
}

int kernel::ident() const
{
    return _fd;
}

int kernel::add_fd( int fd, short event )
{
    return WSAAsyncSelect(fd, (HWND)_fd, KERNEL_IO_MSG, event);
}

int kernel::remove_fd( int fd, short event )
{
    return WSAAsyncSelect(fd, (HWND)_fd, KERNEL_IO_MSG, 0);
}

} //namespace mio