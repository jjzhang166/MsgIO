#include "test.h"
#include <cclog/cclog_debugoutput.h>
#include <iostream>

using namespace std;

int main(void) {
    cclog::reset(new cclog_debugoutput(cclog::TRACE));
    try
    {
        test_timer();
        test_submit();
    }
    catch (std::exception &e)
    {
        cout << e.what() << endl;
    }
    return 0;
}