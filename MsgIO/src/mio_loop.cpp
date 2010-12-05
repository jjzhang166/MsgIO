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
    _writer.run(bind(&loop::thread_main, this));
}

void loop::run()
{
    start();
    _writer.join();
}

void loop::thread_main()
{
    MSG msg;
    HWND wnd = _kernel.ident();
    while (GetMessage (&msg, wnd, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
}

} //namespace mio