#include <iostream>
#include "mio_loop.h"

using namespace std;

int main(void) {
    mio::loop loop;
    loop.start();
    loop.join();
    return 0;
}