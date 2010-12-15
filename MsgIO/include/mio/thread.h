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
    bool wait(int timeout = -1);
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

class thread_cond
{
private: //Disable copy and assignment
    thread_cond(const thread_cond&);         
    void operator=(const thread_cond&);

public:
    thread_cond()
    {
        _semaphore = ::CreateSemaphore(NULL, 0, LONG_MAX, NULL);
        if (_semaphore == NULL)
            throw thread_error(GetLastError(), "error!");

        _num_waiting = 0;
        _num_wake = 0;
        _generation = 0;

        ::InitializeCriticalSection(&_csection);
    }

    ~thread_cond()
    {
        ::CloseHandle(_semaphore);
        ::DeleteCriticalSection(&_csection);
    }

    bool wait(thread_mutex& lock)
    {
        return do_wait(lock, INFINITE);
    }

//     bool timedwait(thread_mutex& lock, const struct timespec *abstime)
//     {
//         int milliseconds = timespec_to_ms(abstime);
//         return do_wait(lock, milliseconds);
//     }

    bool signal()
    {
        unsigned int wake = 0;

        ::EnterCriticalSection(&_csection);
        if (_num_waiting > _num_wake)
        {
            wake = 1;
            _num_wake++;
            _generation++;
        }
        ::LeaveCriticalSection(&_csection);

        if (wake)
            ::ReleaseSemaphore(_semaphore, 1, NULL);

        return true;
    }

    bool broadcast()
    {
        unsigned long num_wake = 0;

        ::EnterCriticalSection(&_csection);
        if (_num_waiting > _num_wake)
        {
            num_wake = _num_waiting - _num_wake;
            _num_wake = _num_waiting;
            _generation++;
        }
        ::LeaveCriticalSection(&_csection);

        if (num_wake)
            ::ReleaseSemaphore(_semaphore, num_wake, NULL);

        return true;
    }

private:
    bool do_wait(thread_mutex& lock, unsigned milliseconds)
    {
        unsigned long res;
        bool ret;
        unsigned int wake = 0;
        unsigned long generation;

        ::EnterCriticalSection(&_csection);
        _num_waiting++;
        generation = _generation;
        ::LeaveCriticalSection(&_csection);

        lock.unlock();

        do
        {
            res = ::WaitForSingleObject(_semaphore, milliseconds);
            ::EnterCriticalSection(&_csection);

            if (_num_wake)
            {
                if (_generation != generation)
                {
                    _num_wake--;
                    _num_waiting--;
                    ret = true;
                    break;
                }
                else
                {
                    wake = 1;
                }
            }
            else if (res != WAIT_OBJECT_0)
            {
                _num_waiting--;
                ret = false;
                break;
            }

            ::LeaveCriticalSection(&_csection);

            if (wake)
            {
                wake = 0;
                ::ReleaseSemaphore(_semaphore, 1, NULL);
            }
        } while (true);

        ::LeaveCriticalSection(&_csection);
        lock.lock();

        return ret;
    }


    HANDLE                _semaphore;
    CRITICAL_SECTION       _csection;
    unsigned long        _num_waiting;
    unsigned long        _num_wake;
    unsigned long        _generation;
};

} //namespace mio