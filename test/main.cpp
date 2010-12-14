#include "test.h"
#include <cclog/cclog_debugoutput.h>
#include <cclog/cclog_ostream.h>
#include <iostream>

using namespace std;

int main(void) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    //cclog::reset(new cclog_debugoutput(cclog::TRACE));
    cclog::reset(new cclog_ostream(cclog::TRACE, cout));
    try
    {
        test_timer();
        test_submit();
        test_listen_conn();
    }
    catch (std::exception &e)
    {
        cout << e.what() << endl;
    }
    WSACleanup();
    return 0;
}