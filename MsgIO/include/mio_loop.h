#pragma once

#include "mio_queue.h"

namespace mio {

class loop {
public:
    loop();
    ~loop();


private:
    queue _queue;
};

} // namespace mio