#include "mio_thread.h"
#include <iostream>

namespace mio {
thread_error::thread_error( int errno_, const std::string& msg ) :system_error(errno_, msg) {}

DWORD WINAPI thread::trampoline( void* user )
{
    try {
        std::auto_ptr<function_t> f(reinterpret_cast<function_t*>(user));
        (*f)();
    } catch (std::exception& e) {
        std::cerr
            << "thread terminated with throwing an instance of '"
            << typeid(e).name()
            << "'\n"
            << "  what():  " << e.what() << std::endl;
    }
    return 0;
}

thread::thread()
{

}

thread::~thread()
{

}

void thread::run( function_t func )
{
    std::auto_ptr<function_t> f(new function_t(func));
    create(&trampoline, reinterpret_cast<void*>(f.get()));
    f.release();
}

void thread::create( DWORD (WINAPI *func)(void*), void* user )
{
    DWORD threadId;
    _thread = CreateThread(NULL, 0, func, user, 0, &threadId);
    if(_thread == NULL) { throw thread_error(GetLastError(), "failed to create thread"); }
}

void thread::join()
{
    DWORD ret = WaitForSingleObject(_thread, INFINITE);
    if(ret != WAIT_OBJECT_0) { throw thread_error(GetLastError(), "failed to join thread"); }
}

void thread::detach()
{
    BOOL ret = CloseHandle(_thread);
    if(!ret) { throw thread_error(GetLastError(), "failed to detach thread"); }
}

bool thread::isEqual( const thread& other )
{
    return _thread == other._thread;
}

void thread::exit( void *retVal )
{
    ::ExitThread((DWORD)retVal);
}
} //namespace mio