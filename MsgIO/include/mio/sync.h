#pragma once

#include "mio/thread.h"

namespace mio {


template <typename T>
class sync {
public:
	sync() : _obj() { }
	template <typename A1>
	sync(A1 a1) : _obj(a1) { }
	template <typename A1, typename A2>
	sync(A1 a1, A2 a2) : _obj(a1, a2) { }
	template <typename A1, typename A2, typename A3>
	sync(A1 a1, A2 a2, A3 a3) : _obj(a1, a2, a3) { }
	template <typename A1, typename A2, typename A3, typename A4>
	sync(A1 a1, A2 a2, A3 a3, A4 a4) : _obj(a1, a2, a3, a4) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) : _obj(a1, a2, a3, a4, a5) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) : _obj(a1, a2, a3, a4, a5, a6) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) : _obj(a1, a2, a3, a4, a5, a6, a7) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) : _obj(a1, a2, a3, a4, a5, a6, a7, a8) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) : _obj(a1, a2, a3, a4, a5, a6, a7, a8, a9) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) : _obj(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11) : _obj(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12) : _obj(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13) : _obj(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14) : _obj(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15) : _obj(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) { }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	sync(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16) : _obj(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16) { }

	~sync() { }

	T& unsafe_ref() { return _obj; }
	const T& unsafe_ref() const { return _obj; }

	class ref {
	public:
		ref(sync<T>& obj) : _ref(NULL)
		{
			obj._mutex.lock();
			_ref = &obj;
		}
	
		ref() : _ref(NULL) { }
	
		~ref() { reset(); }
	
		void reset()
		{
			if(_ref) {
				_ref->_mutex.unlock();
				_ref = NULL;
			}
		}
	
		void reset(sync<T>& obj)
		{
			reset();
			obj._mutex.lock();
			_ref = &obj;
		}

		void swap(sync<T>& obj)
		{
			sync<T>* tmp = _ref;
			_ref = obj._ref;
			obj._ref = tmp;
		}
	
		T& operator*() { return _ref->_obj; }
		T* operator->() { return &operator*(); }
		const T& operator*() const { return _ref->_obj; }
		const T* operator->() const { return &operator*(); }

		operator bool() const { return _ref != NULL; }

		thread_mutex& get_mutex()
		{
			return _ref->_mutex;
		}
	
	protected:
		sync<T>* _ref;
	
	private:
		ref(const ref&);
	};

	class auto_ref : public ref {
	public:
		auto_ref(sync<T>& obj) : ref(obj) { }
		auto_ref() { }
		~auto_ref() { }

		auto_ref(auto_ref& o)
		{
			ref::_ref = o._ref;
			o._ref = NULL;
		}

		auto_ref& operator= (auto_ref& o)
		{
			auto_ref(o).swap(*this);
			return *this;
		}
	};

	auto_ref lock()
	{
		return auto_ref(*this);
	}

private:
	T _obj;
	thread_mutex _mutex;
	friend class ref;

private:
	sync(const sync&);
};


}  // namespace mp


