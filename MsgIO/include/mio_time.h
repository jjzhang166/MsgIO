#pragma once
#include <time.h>

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

inline int timespec_to_ms(const struct timespec *ts)
{
    return (int)ts->tv_sec + ts->tv_nsec / 1000000 + (ts->tv_nsec % 1000000 > 500000);
}
