#pragma once

#include "unwindows.h"
#include "functional.h"

namespace mio {
class queue 
{
private: //Disable copy and assignment
	queue(const queue&);         
	void operator=(const queue&);

public: //Typedefs
    typedef function<int(HWND, UINT, WPARAM, LPARAM)> function_t;

public:
    queue();
    queue(function_t queue_func);

    ~queue();

    void initialize();

    int queue_func(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    function_t _callback;
    HWND _msgWnd;
};

} //namespace mio