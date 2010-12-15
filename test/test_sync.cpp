#include "test.h"
#include <mio/sync.h>
#include <vector>

struct test {
    test(int num1, int num2) :
num1(num1), num2(num2) { }

volatile int num1;
volatile int num2;
};

void thread_main(mio::sync<test>* obj)
{
    for(int i=0; i < 20; ++i) {
        mio::sync<test>::ref ref(*obj);
        ref->num1++;
        ref->num1--;
        std::cout << (ref->num1 + ref->num2);
    }
}

void test_sync()
{
    mio::sync<test> obj(0, 0);

    std::vector<mio::thread> threads(4);
    for(int i=0; i < 4; ++i) {
        threads[i].run(mio::bind(&thread_main, &obj));
    }

    for(int i=0; i < 4; ++i) {
        threads[i].join();
    }

    std::cout << std::endl;
}

