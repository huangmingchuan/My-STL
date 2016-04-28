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

	template <typename T>
	class shared_ptr
	{
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using const_pointer = const T*;
		using const_reference = const T&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
	private:
		T* _ptr;
		shared_count<T> count;
	public:
		shared_ptr() :_ptr(0), count() {}
		shared_ptr(T* p) :_ptr(p), count(p) {}
		template <typename D>
		shared_ptr(T* p, D d) : _ptr(p), count(p, d) {}
		shared_ptr(shared_ptr&& rhs) :_ptr(rhs._ptr), count()
		{
			count.swap(rhs.count);
			rhs._ptr = nullptr;
		}
		shared_ptr(shared_ptr& rhs) = default;
		~shared_ptr() = default;
		shared_ptr& opeartor = (shared_ptr& rhs)
		{
			_ptr = rhs._ptr;
			count = rhs.count;
			return *this;
		}

		void reset()
		{
			shared_ptr().swap(*this);
		}

		reference opeartor*() const
		{
			return *_ptr;
		}

		pointer opeartor->() const
		{
			return &(operator*());
		}

		pointer get() const { return _ptr; }

		explicit operator bool() const
		{
			return _ptr == nullptr ? false : true;
		}

		bool unique() const
		{
			return count.unique();
		}

		long use_count() const
		{
			return count.use_count();
		}

		template <typename SubT>
		void reset(SubT* p)
		{
			shared_ptr(p).swap(*this);
		}

		void swap(shared_ptr<T>& rhs)
		{
			std::swap(_ptr, rhs._ptr);
			count.swap(rhs.count);
		}
	};

	template <typename T>
	bool operator==(shared_ptr<T>& lhs, shared_ptr<T>& rhs)
	{
		return lhs.get() == rhs.get();
	}

	template <typename T>
	bool operator!=(shared_ptr<T>& lhs, shared_ptr<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T, typename ...Args>
	shared_ptr<T> make_shared(Args... args)
	{
		return shared_ptr<T>(new T(std::forward<Args>(args)...));
	}

	template <typename T>
	struct default_deleter
	{
		void operator()(T* p)
		{
			if (p) delete p;
		}
	};

	template <typename T, typename Deleter = default_deleter<T>>
	class unique_ptr
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = std::size_t;
	private:
		T* _ptr;
		Deleter Del;
	public:
		unique_ptr() :_ptr(nullptr), Del(Deleter()) {}
		unique_ptr(pointer p) :_Ptr(p), Del(Deleter()) {}
		unique_ptr(pointer p, Deleter&&d) :_Ptr(p), Del(std::move(d)) {}
		unique_ptr(unique_ptr&&rhs) :_Ptr(std::move(rhs._Ptr)), Del(std::move(rhs.Del))
		{
			rhs._Ptr = nullptr;
		}
		unique_ptr& operator=(unique_ptr&& rhs)
		{
			_Ptr = std::move(rhs._Ptr);
			Del = std::move(rhs.Del);
			rhs._Ptr = nullptr;
		}
		~unique_ptr()
		{
			Del(_ptr);
		}
		unique_ptr(unique_ptr& rhs) = delete;
		unique_ptr& operator=(unique_ptr& rhs) = delete;

		reference operator*() const
		{
			return *_ptr;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		pointer get() const
		{
			return _ptr;
		}

		pointer release()
		{
			pointer temp = _ptr;
			_ptr = nullptr;
			return temp;
		}

		void reset(pointer p = nullptr)
		{
			pointer temp = _ptr;
			_ptr = p;
			if (temp) Del(temp);
		}

		void swap(unique_ptr& rhs)
		{
			std::swap(_ptr, rhs._ptr);
			std::swap(Del, rhs.Del);
		}
	};

	template <typename T>
	bool operator==(unique_ptr<T>& lhs, unique_ptr<T>& rhs)
	{
		return lhs.get() == rhs.get();
	}

	template <typename T>
	bool operator!=(unique_ptr<T>& lhs, unique_ptr<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T>
	void swap(unique_ptr<T>&rhs, unique_ptr<T>&lhs)
	{
		rhs.swap(lhs);
	}
}

#endif