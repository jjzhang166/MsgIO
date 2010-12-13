#pragma once

#ifdef MP_MEMORY_BOOST
#include <boost/tr1/memory>
namespace mio {
    using std::tr1::shared_ptr;
    using std::tr1::wak_ptr;
    //using std::tr2::scoped_ptr;
    using std::tr1::static_pointer_cast;
    using std::tr1::dynamic_pointer_cast;
    using std::tr1::enable_shared_from_this;
}
#else
#ifdef MP_MEMORY_BOOST_ORG
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
//#include <boost/scoped_ptr.hpp>
namespace mio {
    using boost::shared_ptr;
    using boost::weak_ptr;
    //using boost::scoped_ptr;
    using boost::static_pointer_cast;
    using boost::dynamic_pointer_cast;
    using boost::enable_shared_from_this;
}
#else
#ifndef MP_MEMORY_STANDARD
#include <tr1/memory>
namespace mio {
    using std::tr1::shared_ptr;
    using std::tr1::weak_ptr;
    //using std::tr2::scoped_ptr;
    using std::tr1::static_pointer_cast;
    using std::tr1::dynamic_pointer_cast;
    using std::tr1::enable_shared_from_this;
}
#else
#include <memory>
namespace mio {
    using std::shared_ptr;
    using std::weak_ptr;
    //using std::scoped_ptr;
    using std::static_pointer_cast;
    using std::dynamic_pointer_cast;
    using std::enable_shared_from_this;
}
#endif
#endif
#endif

