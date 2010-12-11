#include "test.h"

using namespace mio;

void submit_callback() {
    LOG_DEBUG("Got callback");
}

void submit_callback2(int a, int b) {
    LOG_DEBUG("result:", a+b);
}

void test_submit()
{
    loop l;
    l.submit(submit_callback);
    l.submit(submit_callback2, 3, 4);
    l.submit(&loop::end, &l);
    l.run();
}