#pragma once

#ifdef UNDER_CE
#include "cclog.h"
#include <windows.h>

class cclog_serial : public cclog {
public:
    cclog_serial(level runtime_level) : cclog(runtime_level) {}
    ~cclog_serial(){};

    void log_impl(level lv, std::string& str) {
        std::wstring wstr(str.length(), L'');
        std::copy(str.begin(), str.end(), wstr.begin());
        RETAILMSG(1, (TEXT("%s\n"), wstr.c_str()));
    }
};

#endif