#ifndef MIO_UTILIZE_H__
#define MIO_UTILIZE_H__


#define MIO_UTILIZE \
struct mio_util; \
    friend struct mio_util

#define MIO_UTIL_DEF(name) \
struct name::mio_util : public name

#define MIO_UTIL_IMPL(name) \
    name::mio_util

#define MIO_UTIL \
    (*static_cast<mio_util*>(this))

#define MIO_UTIL_FROM(self) \
    (*static_cast<mio_util*>(self))


#endif /* mio/utilze.h */