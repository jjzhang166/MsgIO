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
    BOOL ret = CloseHandle(_thread);
    if(!ret) { throw thread_error(GetLastError(), "failed to detach thread"); }
}

void thread::run( function_t func )
{
    std::auto_ptr<function_t> f(new function_t(func));
    create(&trampoline, reinterpret_cast<void*>(f.get()));
    f.release();
}

void thread::create( DWORD (WINAPI *func)(void*), void* user )
{
    _thread = CreateThread(NULL, 0, func, user, 0, (LPDWORD)&_id);
    if(_thread == NULL) { throw thread_error(GetLastError(), "failed to create thread"); }
}

void thread::join()
{
    DWORD ret = WaitForSingleObject(_thread, INFINITE);
    if(ret != WAIT_OBJECT_0) { throw thread_error(GetLastError(), "failed to join thread"); }
}

int thread::ident()
{
    return (int)_thread;
}

bool thread::isEqual( const thread& other )
{
    return _id == other._id;
}

void thread::exit( void *retVal )
{
    ::ExitThread((DWORD)retVal);
}

thread_mutex::thread_mutex()
{
    BOOL ret = InitializeCriticalSectionAndSpinCount(&_mutex, 4000);
    if (!ret)
        throw thread_error(GetLastError(), "cannot create mutex");
}

thread_mutex::~thread_mutex()
{
    DeleteCriticalSection(&_mutex);
}

void thread_mutex::lock()
{
    EnterCriticalSection(&_mutex);
}

bool thread_mutex::trylock()
{
    return !!TryEnterCriticalSection(&_mutex);
}

void thread_mutex::unlock()
{
    LeaveCriticalSection(&_mutex);
}
} //namespace mio