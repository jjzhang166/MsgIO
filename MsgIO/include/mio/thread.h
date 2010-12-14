#pragma once

#include "mio/exception.h"
#include "mio/functional.h"
#include "mio/unwindows.h"

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

    int ident();
    bool isEqual(const thread& other);
    
    static void exit(void *retVal = NULL);
private:
    void create(DWORD (WINAPI *func)(void*), void* user);

private:
    HANDLE _thread;
    int _id;

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

class thread_semaphore 
{
private: //Disable copy and assignment
	thread_semaphore(const thread_semaphore&);         
	void operator=(const thread_semaphore&);

public:
    thread_semaphore(int init = 0, int maximum = 65535);

    ~thread_semaphore();

public:
    void wait(int timeout = -1);
    void signal(int count = 1);
    int ident();

private:
    HANDLE _semaphore;
};

class thread_scoped_lock {
public:
    thread_scoped_lock() : _mutex(NULL) { }

    thread_scoped_lock(thread_mutex& mutex) : _mutex(NULL)
    {
        mutex.lock();
        _mutex = &mutex;
    }

    ~thread_scoped_lock()
    {
        if(_mutex) {
            _mutex->unlock();
        }
    }

public:
    void unlock()
    {
        if(_mutex) {
            _mutex->unlock();
            _mutex = NULL;
        }
    }

    void relock(thread_mutex& mutex)
    {
        unlock();
        mutex.lock();
        _mutex = &mutex;
    }

    bool owns() const
    {
        return _mutex != NULL;
    }

private:
    thread_mutex* _mutex;
private:
    thread_scoped_lock(const thread_scoped_lock&);
};

} //namespace mio