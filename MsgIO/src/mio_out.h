#pragma once
#include "mio/mio.h"
#include "mio/sync.h"
#include "kernel.h"
#include <hash_map>

namespace mio {
class loop_impl;

class out {
private: //Disable copy and assignment
    out(const out&);         
    void operator=(const out&);

public:
    out(loop_impl *l);
    ~out();

    typedef loop::finalize_t finalize_t;

    inline void commit_raw(int fd, char* xfbuf, char* xfendp);

    // optimize
    inline void commit(int fd, xfer* xf);
    inline void write(int fd, const void* buf, size_t size);

    void on_write(kernel::event &e);
private:
    void watch(int fd);
    volatile long _watching;

private:
    typedef shared_ptr<xfer> shared_xfer;
    typedef stdext::hash_map<int, shared_ptr<xfer>> xfer_map_t;
    sync<xfer_map_t> _xfers;
    loop_impl *_loop;
}; 
} //namespace mio