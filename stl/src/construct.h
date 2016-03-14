#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new>
#include <type_traits>

namespace mystl
{
	template<typename T1, typename T2>
	inline void construct(T1 *ptr1, const T2& value)
	{
		new (ptr1)T1(value);
	}

	template<typename T>
	inline void destroy(T *ptr)
	{
		ptr->~T();
	}

	template<typename ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, std::true_type) {}

	template<typename ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, std::false_type)
	{
		for (; first != last; ++first)
		{
			destroy(&*first);
		}
	}

	template<typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		typedef typename std::is_pod<ForwardIterator> pod_type;
		_destroy(first, last, pod_type);
	}
}

#endif