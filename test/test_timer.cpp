#include "test.h"

using namespace mio;

std::auto_ptr<loop> l;

bool timer_callback() {
    static int a = 1;
    LOG_DEBUG("got timer ", a);
    if (++a ==5 ) {
        l->submit(&loop::end, l.get());
        return false;
    }
    return true;
}

void test_timer() {
    l.reset(new loop());
    l->add_timer(1, 2, timer_callback);
    l->run();
    l.reset(NULL);
}