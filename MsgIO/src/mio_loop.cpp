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
    _queue.setCallback(bind(&loop::queue_callback, this, _1, _2, _3, _4));
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

int loop::queue_callback( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    std::cout << "got message : " << message << std::endl;
    return 0;
}

void loop::thread_main()
{
    MSG msg;
    _queue.initialize();
    HWND wnd = _queue.getHandle();
    while (GetMessage (&msg, wnd, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
}

} //namespace mio