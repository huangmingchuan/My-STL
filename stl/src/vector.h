#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <type_traits>

#include "allocator.h"
#include "iterator.h"
#include "algorithm.h"
#include "reverse_iterator.h"
#include "uninitialized_functions.h"

namespace mystl
{
	template <typename T, typename Alloc = allocator<T>>
	class vector
	{
	private:
		T *start_;
		T *finishi_;
		T *endOfStorage_;

		typedef Alloc dataAllocator;

	public:
		typedef T value_type;
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef reverse_iterator_t<T*> reverse_iterator;
		typedef reverse_iterator_t<const T*> const_reverse_iterator;
		typedef iterator pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

	public:
		//构造、拷贝、赋值、析构函数
		vector() :start_(nullptr), finishi_(nullptr), endOfStorage_(nullptr) {}
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		template <typename InputIterator>
		vector(InputIterator first, InputIterator last);
		vector(const vector& v);
		vector(vector&& v);
		vector& operator=(const vector& v);
		vector& operator=(vector&& v);
		~vector();

		bool operator==(const vector& v) const;
		bool operator!=(const vector& v) const;

		iterator begin() { return start_; }
		const_iterator begin() const { return start_; }
		const_iterator cbegin() const { return start_; }
		iterator end() { return finishi_; }
		const_iterator end() const { return finishi_; }
		const_iterator cend() const { return finishi_; }
		// const vector 对象 的 rbegin() 和 rend() 成员
		reverse_iterator rbegin() { return reverse_iterator(finishi_); }
		reverse_iterator rend() { return reverse_iterator(start_); }
		const_reverse_iterator rcbegin() const { return const_reverse_iterator(finishi_); }
		const_reverse_iterator rcend() const { return const_reverse_iterator(start_); }
		
		difference_type size() const { return finishi_ - start_; }
		difference_type capacity() const { return endOfStorage_ - start_; }
		bool empty() const { return finishi_ == start_; }
		void resize(size_type n, value_type val = value_type());
		void reserve(size_type n);
		void shrink_to_fit();

		reference operator[](const difference_type i) { return *(begin() + i); }
		const_reference operator[](const difference_type i) const { return *(cbegin() + i); }
		// const vector 对象的 front back 成员
		reference front() { return *(begin()); }
		reference back() { return *(end() - 1); }
		pointer data() { return start_; }

		void clear();
		void swap(vector& v);
		void push_back(const value_type& value);
		void pop_back();
		iterator insert(iterator position, const value_type& value);
		void insert(iterator position, const size_type& n, const value_type& value);
		template <typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		Alloc get_allocator() { return dataAllocator; }

	private:
		void destroyAndDeallocateAll();
		void allocateAndFillN(const size_type n, const value_type& value);
		template <typename InputIterator>
		// 这个用在哪
		void allocateAndCopy(InputIterator first, InputIterator last);

		template <typename InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);
		template <typename Integer>
		void vector_aux(Integer n, const value_type& value, std::true_type);
		template <typename InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template <typename Integer>
		void insert_aux(iterator position, Integer n, const value_type& value, std::true_type);
		template <typename InputIterator>
		void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
		void reallocateAndFillN(iterator position, const size_type& n, const value_type& value);
		size_type getNewCapacity(size_type len) const;

	public:
		template <typename T, typename Alloc>
		friend bool operator==(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
		template <typename T, typename Alloc>
		friend bool operator!=(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
	};

	//构造、拷贝、赋值、析构函数
	template <typename T, typename Alloc>
	vector<T, Alloc>::~vector()
	{
		destroyAndDeallocateAll();
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const size_type n)
	{
		allocateAndFillN(n, value_type());
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value)
	{
		allocateAndFillN(n, value);
	}
	
	template <typename T, typename Alloc>
	template <typename InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last)
	{
		//处理指针和数值之间的区别
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const vector& v)
	{
		allocateAndCopy(v.start_, v.finishi_);
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(vector&& v)
	{
		start_ = v.start_;
		finishi_ = v.finishi_;
		endOfStorage_ = v.endOfStorage_;
		v.start_ = v.finishi_ = v.endOfStorage_ == nullptr;
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v)
	{
		if (this != &v)
		{
			allocateAndCopy(v.start_, v.finishi_);
		}
		return *this;
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& v)
	{
		if (this != &v)
		{
			destroyAndDeallocateAll();
			start_ = v.start_;
			finishi_ = v.finishi_;
			endOfStorage_ = v.endOfStorage_;
			v.start_ = v.finishi_ = v.endOfStorage_ = nullptr;
		}
		return *this;
	}


}

#endif