#pragma once

#include "unwindows.h"

namespace mio {
class kernel 
{
private: //Disable copy and assignment
	kernel(const kernel&);         
	void operator=(const kernel&);

public:
    kernel();

    ~kernel();

    HWND ident();

private:
    HWND _msgWnd;
};

} //namespace mio