#ifndef MIO_FUNCTIONAL_H__
#define MIO_FUNCTIONAL_H__

#ifdef MIO_FUNCTIONAL_BOOST
#include <boost/tr1/functional.hpp>
namespace mio {
	using std::tr1::function;
	using std::tr1::bind;
	namespace placeholders {
		using namespace std::tr1::placeholders;
	}
}
#else
#ifdef MIO_FUNCTIONAL_BOOST_ORG
#include <boost/function.hpp>
#include <boost/bind.hpp>
namespace mio {
	using boost::function;
	using boost::bind;
	namespace placeholders { }
}
#else
#ifndef MIO_FUNCTIONAL_STANDARD
#include <tr1/functional>
namespace mio {
	using std::tr1::function;
	using std::tr1::bind;
	namespace placeholders {
		using namespace std::tr1::placeholders;
	}
}
#else
#include <functional>
namespace mio {
	using std::function;
	using std::bind;
	namespace placeholders {
		using namespace std::placeholders;
	}
}
#endif
#endif
#endif

#endif /* mp/functional.h */

