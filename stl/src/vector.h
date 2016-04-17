#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <initializer_list>

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
		vector(const std::initializer_list<T>& l);
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
	vector<T, Alloc>::vector(const std::initializer_list<T>& l)
	{
		vector_aux(l.begin(), l.end(), std::false_type());
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
		std::cout << "copy constructor " << std::endl;
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(vector&& v)
	{
		start_ = v.start_;
		finishi_ = v.finishi_;
		endOfStorage_ = v.endOfStorage_;
		v.start_ = v.finishi_ = v.endOfStorage_ = nullptr;
		std::cout << "move constructor " << std::endl;
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v)
	{
		if (this != &v)
		{
			allocateAndCopy(v.start_, v.finishi_);
		}
		std::cout << "copy assignment " << std::endl;
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
		std::cout << "move assignment " << std::endl;
		return *this;
	}

	//容量相关
	template <typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_type n, value_type val = value_type())
	{
		if (n < size())
		{
			dataAllocator::destroy(start_ + n, finishi_);
			finishi_ = start_ + n;
		}
		else if (n > size() && n <= capacity())
		{
			auto lenOfInsert = n - size();
			finishi_ = mystl::uninitialized_fill_n(finishi_, lenOfInsert, val);
		}
		else if (n > capacity())
		{
			auto lenOfInsert = n - size();
			T *newStart = dataAllocator::allocate(getNewCapacity(lenOfInsert));
			T *newFinish = mystl::uninitialized_copy(begin(), end(), newStart);
			newFinish = mystl::uninitialized_fill_n(newFinish, lenOfInsert, val);
			destroyAndDeallocateAll();
			start_ = newStart;
			finishi_ = newFinish;
			endOfStorage_ = start_ + n;
		}
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::reserve(size_type n)
	{
		if (n <= capacity()) return;
		T *newStart = dataAllocator::allocate(n);
		T *newFinish = mystl::uninitialized_copy(begin(), end(), newStart);
		destroyAndDeallocateAll();
		start_ = newStart;
		finishi_ = newFinish;
		endOfStorage_ = start_ + n;
	}
	
	// 修改容器相关
	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position)
	{
		return erase(position, position + 1);
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last)
	{
		//尾部残留对象数
		difference_type lenOfTail = end() - last;
		//删除的对象数
		difference_type lenOfRemove = last - first;
		finishi_ = finishi_ - lenOfRemove;
		for (; lenOfTail != 0; --lenOfTail)
		{
			auto temp = last - lenOfRemove;
			*temp = *(last++);
		}
		return first;
	}

	template <typename T, typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::reallocateAndCopy(iterator position, InputIterator first, InputIterator last)
	{
		difference_type newCapacity = getNewCapacity(last - first);
		T *newStart = dataAllocator::allocate(newCapacity);
		T *newEndOfStorage = newStart + newCapacity;
		T *newFinish = mystl::uninitialized_copy(begin(), position, newStart);
		newFinish = mystl::uninitialized_copy(first, last, newFinish);
		newFinish = mystl::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		start_ = newStart;
		finishi_ = newFinish;
		endOfStorage_ = newEndOfStorage;
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::reallocateAndFillN(iterator position, const size_type& n, const value_type& val)
	{
		difference_type newCapacity = getNewCapacity(n);
		T *newStart = dataAllocator::allocate(newCapacity);
		T *newEndOfStorage = newStart + newCapacity;
		T *newFinish = mystl::uninitialized_copy(begin(), position, newStart);
		newFinish = mystl::uninitialized_fill_n(newFinish, n, val);
		newFinish = mystl::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		start_ = newStart;
		finishi_ = newFinish;
		endOfStorage_ = newEndOfStorage;
	}

	template <typename T, typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type)
	{
		difference_type locationLeft = endOfStorage_ - finishi_;
		difference_type locationNeed = std::distance(first, last);

		if (locationLeft >= locationNeed)
		{
			if (finishi_ - position > locationNeed)
			{
				mystl::uninitialized_copy(finishi_ - locationNeed, finishi_, finishi_);
				std::copy_backward(position, finishi_ - locationNeed, finishi_);
				std::copy(first, last, position);
			}
			else
			{
				iterator temp = mystl::uninitialized_copy(first + (finishi_ - position), last, finishi_);
				mystl::uninitialized_copy(position, finishi_, temp);
				std::copy(first, first + (finishi_ - position), position);
			}
			finishi_ += locationNeed;
		}
		else
		{
			reallocateAndCopy(position, first, last);
		}
	}

	template <typename T, typename Alloc>
	template <typename Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n, const value_type& value, std::true_type)
	{
		assert(n != 0);
		difference_type locationLeft = endOfStorage_ - finishi_;
		difference_type locationNeed = n;

		if (locationLeft >= locationNeed)
		{
			auto temp = end() - 1;
			for (; temp - position >= 0; --temp)
			{
				construct(temp + locationNeed, *temp);
			}
			mystl::uninitialized_fill_n(position, n, value); 
			finishi_ += locationNeed;
		}
		else
		{
			reallocateAndFillN(position, n, value);
		}
	}

	template <typename T, typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last)
	{
		insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::insert(iterator position, const size_type& n, const value_type& val)
	{
		insert_aux(position, n, val, typename std::is_integral<size_type>::type());
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& val)
	{
		const auto index = position - begin();
		insert(position, 1, val);
		return begin() + index;
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::push_back(const value_type& val)
	{
		insert(end(), val);
	}

	//逻辑比较相关
	template <typename T, typename Alloc>
	bool vector<T, Alloc>::operator==(const vector& v) const
	{
		if (size() != v.size()) 
			return false;
		else
		{
			auto p1 = start_;
			auto p2 = v.start_;
			for (; p1 != finishi_ && p2 != v.finishi_; ++p1, ++p2)
			{
				if (*p1 != *p2)
					return false;
			}
			return true;
		}
	}

	template <typename T, typename Alloc>
	bool vector<T, Alloc>::operator!=(const vector& v) const
	{
		return !(*this == v);
	}

	template <typename T, typename Alloc>
	bool operator==(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2)
	{
		return v1.operator==(v2);
	}

	template <typename T, typename Alloc>
	bool operator!=(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2)
	{
		return !(v1 == v2);
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::shrink_to_fit()
	{
		T* t = (T*)dataAllocator::allocate(size());
		finishi_ = mystl::uninitialized_copy(start_, finishi_, t);
		dataAllocator::deallocate(start_, capacity());
		start_ = t;
		endOfStorage_ = finishi_;
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::clear()
	{
		dataAllocator::destroy(start_, finishi_);
		finishi_ = start_;
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::swap(vector& v)
	{
		if (this != &v)
		{
			std::swap(start_, v.start_);
			std::swap(finishi_, v.finishi_);
			std::swap(endOfStorage_, v.endOfStorage_);
		}
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::pop_back()
	{
		--finishi_;
		dataAllocator::destroy(finishi_);
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::destroyAndDeallocateAll()
	{
		if (capacity() != 0)
		{
			dataAllocator::destroy(start_, finishi_);
			dataAllocator::deallocate(start_, capacity());
		}
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type& value)
	{
		start_ = dataAllocator::allocate(n);
		mystl::uninitialized_fill_n(start_, n, value);
		finishi_ = endOfStorage_ = start_ + n;
	}

	template <typename T, typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::allocateAndCopy(InputIterator first, InputIterator last)
	{
		start_ = dataAllocator::allocate(last - first);
		finishi_ = mystl::uninitialized_copy(first, last, start_);
		endOfStorage_ = finishi_;
	}

	template <typename T, typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type)
	{
		allocateAndCopy(first, last);
	}

	template <typename T, typename Alloc>
	template <typename Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type)
	{
		allocateAndFillN(n, value);
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len) const
	{
		size_type oldCapacity = endOfStorage_ - start_;
		auto res = std::max(oldCapacity, len);
		size_type newCapacity = oldCapacity != 0 ? (oldCapacity + res) : len;
		return newCapacity;
	}
}

#endif