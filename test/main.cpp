#include "test.h"
#include <cclog/cclog_debugoutput.h>
#include <cclog/cclog_ostream.h>
#include <iostream>

using namespace std;

#ifdef UNDER_CE
#include <cclog/cclog_serial.h>
int _tmain() {
#else
int main(void) {
#endif
    mio::init();
#ifdef UNDER_CE
    cclog::reset(new cclog_serial(cclog::TRACE));
#else
    cclog::reset(new cclog_ostream(cclog::TRACE, cout));
#endif
    //cclog::reset(new cclog_debugoutput(cclog::TRACE));
    try
    {
        test_timer();
        test_submit();
        test_listen_conn();
        test_sync();
    }
    catch (std::exception &e)
    {
        cout << e.what() << endl;
    }
    return 0;
}