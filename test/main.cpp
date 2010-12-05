#include <iostream>
#include "mio_queue.h"

using namespace std;
LRESULT CALLBACK callback (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    cout << "got message " << message << endl;
    return 0;
}

int main(void) {
    mio::queue queue(callback);
    queue.initialize();
    queue.dispatch();
    return 0;
}