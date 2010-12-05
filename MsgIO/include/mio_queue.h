#pragma once

#include "unwindows.h"

namespace mio {
class queue 
{
private: //Disable copy and assignment
	queue(const queue&);         
	void operator=(const queue&);

public:
    queue();

    ~queue();

    HWND getHandle();

private:
    HWND _msgWnd;
};

} //namespace mio