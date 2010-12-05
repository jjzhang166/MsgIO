#pragma once

#include "mio_exception.h"
#include "functional.h"
#include "unwindows.h"

namespace mio {
class thread_error : system_error {
public:
	thread_error(int errno_, const std::string& msg);
};

class thread {
private: //Disable copy and assignment
	thread(const thread&);         
	void operator=(const thread&);

public:
    typedef function<void()> function_t;

public:
    thread();
    ~thread();

    void run(function_t func);
    void join();
    void detach();

    bool isEqual(const thread& other);
    
    static void exit(void *retVal = NULL);

private:
    void create(DWORD (WINAPI *func)(void*), void* user);

private:
    HANDLE _thread;

    static DWORD WINAPI trampoline(void* user);
};

class thread_mutex 
{
private: //Disable copy and assignment
	thread_mutex(const thread_mutex&);         
	void operator=(const thread_mutex&);

public:
    thread_mutex();

    ~thread_mutex();

public:
    void lock();

    bool trylock();

    void unlock();

private:
    CRITICAL_SECTION _mutex;
};

} //namespace mio