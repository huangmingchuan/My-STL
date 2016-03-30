#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "allocator.h"
#include "iterator.h"
#include "uninitialized_functions.h"

namespace mystl
{
	namespace
	{
		template <typename T>
		class vector_iterator : public iterator<random_access_iterator<T, ptrdiff_t>, T>
		{
		private:
			T* ptr_;

		public:
			vector_iterator() :ptr_(nullptr) {}
			explicit vector_iterator(T* p) :ptr_(p) {}
			vector_iterator(const vector_iterator& iter);
			vector_iterator& operator=(const vector_iterator& iter);

			T& operator*() const { return *ptr_; }
			T* operator->() const { return &this->operator*(); }

			vector_iterator& operator++() { ++ptr_; return *this; }
			vector_iterator operator++(int) { auto temp = *this; ++*this; return temp; }
			vector_iterator& operator--() { --ptr_; return *this; }
			vector_iterator operator--(int) { auto tempp = *this; --*this; return temp; }

			bool operator==(const vector_iterator& it) { return ptr_ == it.ptr_; }
			bool operator!=(const vector_iterator& it) { return !(*this == it); }
			typename vector_iterator::difference_type operator-(const vector_iterator& it) { return ptr_ - it.ptr_; }

			friend typename vector_iterator::difference_type operator-(const vector_iterator&, const vector_iterator&);
		};

		template <typename T>
		vector_iterator<T>::vector_iterator(const vector_iterator& it)
		{
			ptr_ = it.ptr_;
		}

		template <typename T>
		vector_iterator<T>& vector_iterator<T>::operator=(const vector_iterator& it)
		{
			if (this != &it)
				ptr_ = it.ptr_;
		}
	} //end of anonymous namespace

	template <typename T, typename Alloc = allocator<T>>
	class vector
	{
	private:
		T* start_;
		T* finish_;
		T* endOfStorage_;

		typedef Alloc dataAllocator;

	public:
		typedef T value_type;
		typedef vector_iterator<T> iterator;
		typedef iterator pointer;
		typedef T& reference;
		typedef size_t size_type;
		typedef typename iterator::difference_type difference_type;

	public:
		//构造、赋值、析构等函数
		vector() :start_(nullptr), finish_(nullptr), endOfStorage_(nullptr) {}
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		template <typename It> vector(It begin, It end);

		vector(const vector& v);
		vector(vector&& v);
		vector& operator=(const vector& v);
		~vector()
		{
			dataAllocator::destroy(start_, finish_);
			dataAllocator::deallocate(start_, endOfStorage_ - start_);
		}

		//迭代器
		iterator begin() { return iterator(start_); }
		iterator end() { return iterator(finish_); }

	private:
		void allocateAndFill(const size_type n, const value_type& value)
		{
			start_ = dataAllocator::allocate(n);
			uninitialized_fill_n(start_, n, value);
			finish_ = endOfStorage_ = start_ + n;
		}

		template <typename It>
		void allocateAndCopy(It begin, It end)
		{
			start_ = dataAllocator::allocate(end - begin);
			finish_ = uninitialized_copy(begin, end, start_);
			endOfStorage_ = finish_;
		}
	};

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const size_type n)
	{
		allocateAndFill(n, value_type());
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value)
	{
		allocateAndFill(n, value);
	}

	template <typename T, typename Alloc>
	template <typename It>
	vector<T, Alloc>::vector(It begin, It end)
	{
		allocateAndCopy(begin, end);
	}
}

#endif