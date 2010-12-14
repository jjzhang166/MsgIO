#include "mio_out.h"
#include "mio_loop.h"

#include <limits.h>
#include <vector>
#include "mio/unwindows.h"
#include "cclog/cclog.h"

namespace mio {
namespace {
class xfer_impl : public xfer {
public:
	xfer_impl() { }
	~xfer_impl() { }

	bool try_write(int fd);

	void push_xfraw(char* buf, size_t size);

	static size_t sizeof_mem();
    static size_t sizeof_iovec(size_t veclen);
	static size_t sizeof_finalize();

	static char* fill_mem(char* from, const void* buf, size_t size);
    static char* fill_iovec(char* from, const iovec* vec, size_t veclen);
	static char* fill_finalize(char* from, finalize_t fin, void* user);

	static bool execute(int fd, char* head, char** tail);

public:
	thread_mutex& mutex() { return m_mutex; }

private:
	thread_mutex m_mutex;

private:
	xfer_impl(const xfer_impl&);
};

typedef shared_ptr<xfer_impl> shared_xfer_impl;
typedef unsigned int xfer_type;

static const xfer_type XF_IOVCE    = 0;
static const xfer_type XF_FINALIZE = 3;

struct xfer_sendfile {
    int infd;
    uint64_t off;
    size_t len;
};

struct xfer_finalize {
    void (*finalize)(void*);
    void* user;
};


inline size_t xfer_impl::sizeof_mem()
{
    return sizeof(xfer_type) + sizeof(iovec)*1;
}

size_t xfer_impl::sizeof_iovec( size_t veclen )
{
    return sizeof(xfer_type) + sizeof(iovec) * veclen;
}

inline size_t xfer_impl::sizeof_finalize()
{
    return sizeof(xfer_type) + sizeof(xfer_finalize);
}

inline char* xfer_impl::fill_mem(char* from, const void* buf, size_t size)
{
    *(xfer_type*)from = 1 << 1;
    from += sizeof(xfer_type);

    ((struct iovec*)from)->iov_base = const_cast<void*>(buf);
    ((struct iovec*)from)->iov_len  = size;
    from += sizeof(struct iovec);

    return from;
}

char* xfer_impl::fill_iovec( char* from, const iovec* vec, size_t veclen )
{
    *(xfer_type*)from = veclen << 1;
    from += sizeof(xfer_type);

    const size_t iovbufsz = sizeof(iovec) * veclen;
    memcpy(from, vec, iovbufsz);
    from += iovbufsz;

    return from;
}

inline char* xfer_impl::fill_finalize(char* from, finalize_t fin, void* user)
{
    *(xfer_type*)from = XF_FINALIZE;
    from += sizeof(xfer_type);

    ((xfer_finalize*)from)->finalize = fin;
    ((xfer_finalize*)from)->user = user;
    from += sizeof(xfer_finalize);

    return from;
}

void xfer_impl::push_xfraw(char* buf, size_t size)
{
    if(m_free < size) { reserve(size); }
    memcpy(m_tail, buf, size);
    m_tail += size;
    m_free -= size;
}


#define MP_WAVY_XFER_CONSUMED \
    do { \
    size_t left = endp - p; \
    ::memmove(head, p, left); \
    *tail = head + left; \
    } while(0)

bool xfer_impl::execute(int fd, char* head, char** tail)
{
    char* p = head;
    char* const endp = *tail;
    while(p < endp) {
        switch(*(xfer_type*)p) {
case XF_FINALIZE: {
    xfer_finalize* x = (xfer_finalize*)(p + sizeof(xfer_type));
    if(x->finalize) try {
        x->finalize(x->user);
    } catch (...) { }

    p += xfer_impl::sizeof_finalize();
    break; }

default: {  // XF_IOVCE
    size_t veclen = (*(xfer_type*)p) >> 1;
    iovec* vec = (iovec*)(p + sizeof(xfer_type));

    //ssize_t wl = ::writev(fd, vec, veclen);
    int wl = 0;
    if (WSASend(fd, reinterpret_cast<WSABUF*>(vec), veclen, reinterpret_cast<DWORD*>(&wl), NULL, NULL, NULL) != NOERROR) {
        throw system_error(WSAGetLastError(), "send failed");
    }
    if(wl <= 0) {
        MP_WAVY_XFER_CONSUMED;
        int err = WSAGetLastError();
        if(wl < 0 && (err == WSAEWOULDBLOCK || err == WSAEINTR)) {
            return true;
        } else {
            return false;
        }
    }

    for(size_t i=0; i < veclen; ++i) {
        if(static_cast<size_t>(wl) >= vec[i].iov_len) {
            wl -= vec[i].iov_len;
        } else {
            vec[i].iov_base = (((char*)vec[i].iov_base) + wl);
            vec[i].iov_len -= wl;

            if(i == 0) {
                MP_WAVY_XFER_CONSUMED;
            } else {
                p += sizeof_iovec(veclen);
                size_t left = endp - p;
                char* filltail = fill_iovec(head, vec+i, veclen-i);
                ::memmove(filltail, p, left);
                *tail = filltail + left;
            }

            return true;
        }
    }

    p += sizeof_iovec(veclen);

    break; }
        }
    }

    *tail = head;
    return false;
}


bool xfer_impl::try_write(int fd)
{
    char* const alloc_end = m_tail + m_free;
    bool cont = execute(fd, m_head, &m_tail);
    m_free = alloc_end - m_tail;

    if(!cont && !empty()) {
        // error occured
        ::shutdown(fd, SD_BOTH);
    }
    return cont;
}
} //noname namespace

void xfer::reserve(size_t reqsz)
{
    size_t used = m_tail - m_head;
    reqsz += used;
    size_t nsize = (used + m_free) * 2 + 72;  // used + m_free may be 0

    while(nsize < reqsz) { nsize *= 2; }

    char* tmp = (char*)::realloc(m_head, nsize);
    if(!tmp) { throw std::bad_alloc(); }

    m_head = tmp;
    m_tail = tmp + used;
    m_free = nsize - used;
}


void xfer::push_write(const void* buf, size_t size)
{
    size_t sz = xfer_impl::sizeof_mem();
    if(m_free < sz) { reserve(sz); }
    m_tail = xfer_impl::fill_mem(m_tail, buf, size);
    m_free -= sz;
}

void xfer::push_writev(const struct iovec* vec, size_t veclen)
{
    size_t sz = xfer_impl::sizeof_iovec(veclen);
    if(m_free < sz) { reserve(sz); }
    m_tail = xfer_impl::fill_iovec(m_tail, vec, veclen);
    m_free -= sz;
}

void xfer::push_finalize(finalize_t fin, void* user)
{
    size_t sz = xfer_impl::sizeof_finalize();
    if(m_free < sz) { reserve(sz); }
    m_tail = xfer_impl::fill_finalize(m_tail, fin, user);
    m_free -= sz;
}

void xfer::migrate(xfer* to)
{
    if(to->m_head == NULL) {
        // swap
        to->m_head = m_head;
        to->m_tail = m_tail;
        to->m_free = m_free;
        m_tail = m_head = NULL;
        m_free = 0;
        return;
    }

    size_t reqsz = m_tail - m_head;
    if(to->m_free < reqsz) { to->reserve(reqsz); }

    memcpy(to->m_tail, m_head, reqsz);
    to->m_tail += reqsz;
    to->m_free -= reqsz;

    m_free += reqsz;
    m_tail = m_head;
}

void xfer::clear()
{
    for(char* p = m_head; p < m_tail; ) {
        switch(*(xfer_type*)p) {
        case XF_FINALIZE: {
            xfer_finalize* x = (xfer_finalize*)(p + sizeof(xfer_type));
            if(x->finalize) try {
                x->finalize(x->user);
            } catch (...) { }

            p += xfer_impl::sizeof_finalize();
            break; }

        default:  // XF_IOVCE
            p += xfer_impl::sizeof_iovec( (*(xfer_type*)p) >> 1 );
            break;
        }
    }

    //::free(m_head);
    //m_tail = m_head = NULL;
    //m_free = 0;
    m_free += m_tail - m_head;
    m_tail = m_head;
}

namespace {
    typedef stdext::hash_map<int, shared_ptr<xfer>> xfer_map_t;
    inline shared_ptr<xfer_impl> get_xfer(sync<xfer_map_t>& _xfers, int fd) {
        sync<xfer_map_t>::auto_ref ref(_xfers);
        if (ref->find(fd) == ref->end()) {
            (*ref)[fd] = shared_ptr<xfer_impl>(new xfer_impl());
        }

        return boost::shared_static_cast<xfer_impl>((*ref)[fd]);
    }
}

out::out( loop_impl *l ) : _loop(l), _watching(0)
{

}

out::~out()
{

}

void out::commit( int fd, xfer* xf )
{
    shared_xfer_impl ctx = get_xfer(_xfers, fd);
    thread_scoped_lock lock(ctx->mutex());
    
    if (!ctx->empty()) {
        xf->migrate(ctx.get());
    }

    if(static_cast<xfer_impl*>(xf)->try_write(fd)) {
        xf->migrate(ctx.get());
        watch(fd);  // FIXME exception
    }
}

void out::write( int fd, const void* buf, size_t size )
{
    shared_xfer_impl xfer = get_xfer(_xfers, fd);
    thread_scoped_lock lock(xfer->mutex());
    if (xfer->empty()) {
        int wl = ::send(fd, (char*)buf, size, 0);
        if(wl <= 0) {
            int err = WSAGetLastError();
             if(wl == 0 || ( err != WSAEINTR && err != WSAEWOULDBLOCK)) {
                 ::shutdown(fd, SD_BOTH);
                 return;
             }
        } else if(static_cast<size_t>(wl) >= size) {
            return;
        } else {
            buf  = ((const char*)buf) + wl;
            size -= wl;
        }

        xfer->push_write(buf, size);
        watch(fd);  // FIXME exception
    } else {
        xfer->push_write(buf, size);
    }
}

void out::commit_raw( int fd, char* xfbuf, char* xfendp )
{
    shared_xfer_impl ctx(get_xfer(_xfers, fd));
    thread_scoped_lock lk(ctx->mutex());

    if(!ctx->empty()) {
        ctx->push_xfraw(xfbuf, xfendp - xfbuf);
        return;
    }

    if(xfer_impl::execute(fd, xfbuf, &xfendp)) {
        ctx->push_xfraw(xfbuf, xfendp - xfbuf);  // FIXME exception
        watch(fd);  // FIXME exception
    }
}

void out::watch( int fd )
{
    InterlockedIncrement(&_watching);
    int e = EVENT::WRITE;
    if (_loop->isRead(fd)) {
        e |= EVENT::READ;
    }
    WSAAsyncSelect(fd, _loop->_hwnd, MESSAGE::IO_SOCKET, e);
}

void out::on_write( int ident )
{
    shared_xfer_impl ctx(get_xfer(_xfers, ident));
    thread_scoped_lock lock(ctx->mutex());

    bool cont;
    try {
        cont = ctx->try_write(ident);
    } catch (std::exception e) {
        LOG_ERROR(e.what());
        cont = false;
    }
    
    if(!cont) {
        ctx->clear();
        InterlockedDecrement(&_watching);

        int e = 0;
        if (_loop->isRead(ident)) {
            e |= EVENT::READ;
        }
        WSAAsyncSelect(ident, _loop->_hwnd, MESSAGE::IO_SOCKET, e);
    }
}

#define ANON_out static_cast<loop_impl*>(_impl)->_out

void loop::commit(int fd, xfer* xf)
{ ANON_out->commit(fd, xf); }

void loop::write(int fd, const void* buf, size_t size)
{ ANON_out->write(fd, buf, size); }

void loop::write(int fd,
                 const void* buf, size_t size,
                 finalize_t fin, void* user)
{
    char *xfbuf = new char[ xfer_impl::sizeof_mem() + xfer_impl::sizeof_finalize() ];
    char* p = xfbuf;
    p = xfer_impl::fill_mem(p, buf, size);
    p = xfer_impl::fill_finalize(p, fin, user);
    ANON_out->commit_raw(fd, xfbuf, p);
    delete xfbuf;
}

void loop::writev(int fd,
                  const struct iovec* vec, size_t veclen,
                  finalize_t fin, void* user)
{
    char *xfbuf = new char[ xfer_impl::sizeof_iovec(veclen) + xfer_impl::sizeof_finalize() ];
    char* p = xfbuf;
    p = xfer_impl::fill_iovec(p, vec, veclen);
    p = xfer_impl::fill_finalize(p, fin, user);
    ANON_out->commit_raw(fd, xfbuf, p);
    delete xfbuf;
}

} //namespace mio