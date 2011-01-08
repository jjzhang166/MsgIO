#ifndef MIO_OBJECT_DELETE_H__
#define MIO_OBJECT_DELETE_H__

namespace mio {


template <typename T>
static void object_delete(void* obj)
{
    delete reinterpret_cast<T*>(obj);
}


template <typename T>
static void object_destructor(void* obj)
{
    reinterpret_cast<T*>(obj)->~T();
}


}  // namespace mio

#endif /* mio/object_delete.h */

