#include "mio_loop.h"
#include <iostream>

namespace mio {
loop::loop()
{
}

loop::~loop()
{
}

void loop::start()
{
    _thread.run(bind(&loop::thread_main, this));
}

void loop::join()
{
    _thread.join();
}

void loop::run()
{
    start();
    join();
}

void loop::thread_main()
{
    MSG msg;
    HWND wnd = _queue.getHandle();
    while (GetMessage (&msg, wnd, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
}

} //namespace mio