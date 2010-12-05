#pragma once

#include "mio_queue.h"
#include "mio_thread.h"

namespace mio {

class loop 
{
private: //Disable copy and assignment
	loop(const loop&);         
	void operator=(const loop&);

public:
    loop();
    ~loop();

    void start();
    void join();

    void run();

private:
    void thread_main();
    int queue_callback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    queue _queue;
    thread _thread;
};

} // namespace mio