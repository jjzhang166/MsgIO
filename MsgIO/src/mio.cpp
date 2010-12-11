#include "mio.h"
#include "mio_loop_impl.h"

#define IMP (static_cast<loop_impl*>(_impl))

namespace mio {
loop::loop()
{
    _impl = new loop_impl();
}

loop::~loop()
{
    delete IMP;
}

void loop::start()
{
    IMP->start();
}

void loop::run()
{
    IMP->run();
}

mio::shared_handler loop::add_handler_impl( shared_handler sh )
{
    return IMP->add_handler_impl(sh);
}

void loop::remove_handler( int fd )
{
    return IMP->remove_handler(fd);
}

void loop::submit_impl( task_t f )
{
    IMP->submit_impl(f);
}
} //namespace mio