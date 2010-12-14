#include "test.h"

void accepted(int fd, int err)
{
    if(fd < 0) {
        LOG_ERROR("accept error");
        return;
    }

    try {
        LOG_DEBUG("accepted");

        // do something with fd
        closesocket(fd);

    } catch (...) {
        ::closesocket(fd);
        return;
    }
}

void connected(int fd, int err)
{
    if(fd < 0) {
        LOG_ERROR("connect error");
        return;
    }

    try {
        LOG_DEBUG("connected");

        // do something with fd

    } catch (...) { }
    ::closesocket(fd);
}

using namespace mio;
void test_listen_conn() {
    loop lo;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_port = htons(9090);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lo.listen(PF_INET, SOCK_STREAM, 0,
        (struct sockaddr*)&addr, sizeof(addr),
        bind(&accepted, _1, _2));

    lo.start(4);  // run with 4 threads

    {
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        lo.connect(PF_INET, SOCK_STREAM, 0,
            (struct sockaddr*)&addr, sizeof(addr),
            0.0, connected);
    }

    Sleep(10000);

    lo.end();
    lo.join();
}