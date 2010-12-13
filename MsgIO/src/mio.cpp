#include "mio/mio.h"
#include "mio_loop.h"

namespace mio {
loop::loop()
{
    _impl = new loop_impl();
}

loop::~loop()
{
    delete IMPL;
}

void loop::start()
{
    IMPL->start();
}

void loop::run()
{
    IMPL->run();
}

mio::shared_handler loop::add_handler_impl( shared_handler sh )
{
    return IMPL->add_handler_impl(sh);
}

void loop::remove_handler( int fd )
{
    return IMPL->remove_handler(fd);
}

void loop::submit_impl( task_t f )
{
    IMPL->submit_impl(f);
}

void loop::end()
{
    IMPL->end();
}

bool loop::is_end() const
{
    return IMPL->is_end();
}

void loop::join()
{
    IMPL->join();
}
} //namespace mio