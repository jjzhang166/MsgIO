#pragma once

#include "mio_kernel.h"
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

    void run();


private:
    void thread_main();

private:
    kernel _kernel;
    thread _writer;
};

} // namespace mio