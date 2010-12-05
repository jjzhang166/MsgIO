#include <iostream>
#include "mio_loop.h"

using namespace std;

int main(void) {
    mio::loop loop;
    loop.run();
    return 0;
}