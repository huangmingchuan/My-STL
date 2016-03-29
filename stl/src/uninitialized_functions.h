#ifndef _UNINITIALIZED_FUNCTIONS_H_
#define _UNINITIALIZED_FUNCTIONS_H_

#include "construct.h"
#include "iterator.h"
#include "type_traits.h"

#include <algorithm>

namespace mystl
{
	/*
		uninitialized_copy
	*/
	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator, InputIterator, ForwardIterator, _true_type);

	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator, InputIterator, ForwardIterator, _false_type);

	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		typedef typename _type_traits<typename iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _uninitialized_copy_aux(first, last, result, isPODType());
	}

	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type)
	{
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}

	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type)
	{
		int i = 0;
		for (; first != last; ++first, ++i)
		{
			construct(result + i, *first);
		}
		return result + i;
	}

	/*
		uninitialized_fill
	*/
	template <typename ForwardIterator, typename T>
	void _uninitialized_fill_aux(ForwardIterator, ForwardIterator, const T&, _true_type);

	template <typename ForwardIterator, typename T>
	void _uninitialized_fill_aux(ForwardIterator, ForwardIterator, const T&, _false_type);

	template <typename ForwardIterator, typename T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_uninitialized_fill_aux(first, last, value, isPODType());
	}

	template <typename ForwardIterator, typename T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type)
	{
		//TODO
		//换成自己的 fill 函数
		std::fill(first, last, value);
	}

	template <typename ForwardIterator, typename T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type)
	{
		for (; first != last; ++first)
		{
			construct(first, value);
		}
	}

	/*
		uninitialized_fill_n
	*/
	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator, Size, const T&, _true_type);

	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator, Size, const T&, _false_type);

	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value)
	{
		typedef typename _type_traits<T>::is_POD_type isPODType;
		return _uninitialized_fill_n_aux(first, n, value, isPODType());
	}

	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _true_type)
	{
		//TODO
		//换成自己的 fill_n 函数
		return std::fill_n(first, n, value);
	}

	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _false_type)
	{
		int i = 0;
		for (; i != n; ++i)
		{
			construct((T*)(first + i), value);
		}
		return first + i;
	}

}

#endif