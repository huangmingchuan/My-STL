#ifndef _MEMORY_H
#define _MEMORY_H

#include <utility>

namespace mystl
{
	class sp_counted_base
	{
	private:
		unsigned int _count;
	public:
		sp_counted_base() :_count(1) {}
		virtual ~sp_counted_base() {}
		virtual void dispose() = 0;
		virtual void destroy()
		{
			delete this;
		}
		void add_ref_count() { ++_count; }
		void decr_ref_count() { --_count; }
		long use_count() { return _count; }
		bool expired() { return _count == 0; }
	};

	template <typename T>
	class sp_counted_base_impl : public sp_counted_base
	{
	private:
		T* _ptr;
	public:
		sp_counted_base_impl() :_ptr(0) {}
		sp_counted_base_impl(T* p) :_ptr(p) {}

		virtual void dispose() { delete _ptr; }
		virtual void destroy() { delete this; }
		virtual ~sp_counted_base_impl() {}
	};

	template <typename T, typename Deleter>
	class sp_counted_base_deleter_impl : public sp_counted_base
	{
	private:
		T* _ptr;
		Deleter _D;
	public:
		sp_counted_base_deleter_impl(T* p, Deleter d) :_ptr(p), _D(d) {}
		virtual void dispose() { _D(_ptr); }
		virtual void destroy() { delete this; }
		virtual ~sp_counted_base_deleter_impl() {}
	};

	template <typename T>
	class shared_count
	{
	public:
		using value_type = T;
	private:
		sp_counted_base *_count_ptr;
	public:
		shared_count() :_count_ptr(0) {}
		shared_count(T* p) :_count_ptr(0)
		{
			_count_ptr = new sp_counted_base_impl<T>(p);
		}
		template <typename D>
		shared_count(T* p, D d) : _count_ptr(0)
		{
			_count_ptr = new sp_counted_base_deleter_impl<T, D>(p, d);
		}
		~shared_count()
		{
			if (_count_ptr != nullptr)
			{
				_count_ptr->decr_ref_count();

				if (_count_ptr->use_count() == 0)
					_count_ptr->dispose();
			}
		}
		shared_count(const shared_count& r) :_count_ptr(r._count_ptr)
		{
			if (_count_ptr != nullptr) _count_ptr->add_ref_count();
		}
		shared_count& operator=(const shared_count& r) 
		{
			if (this != &r)
			{
				if (r._count_ptr != nullptr) r._count_ptr->add_ref_count();
				if (_count_ptr != nullptr) _count_ptr->decr_ref_count();
				if (_count_ptr->use_count() == 0)_count_ptr->dispose();
				_count_ptr = r._count_ptr;
			}
			return *this;
		}
		void swap(shared_count& rhs)
		{
			sp_counted_base *temp = rhs._count_ptr;
			rhs._count_ptr = _count_ptr;
			_count_ptr = temp;
		}
		long use_count() const
		{
			return _count_ptr == nullptr ? 0 : _count_ptr->use_count();
		}
		bool unique() const
		{
			return use_count() == 0;
		}
	};
}

#endif