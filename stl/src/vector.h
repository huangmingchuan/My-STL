#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <type_traits>

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
		private:
			T* getPtr() { return ptr_; }
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
			vector_iterator operator+(const difference_type n) { return vector_iterator(ptr_ + n); }
			vector_iterator operator-(const difference_type n) { return vector_iterator(ptr_ - n); }
			difference_type operator-(const vector_iterator& it) { return ptr_ - it.ptr_; }
			
			friend vector_iterator operator + (const vector_iterator& vit, const difference_type i);
			friend vector_iterator operator + (const difference_type i, const vector_iterator& vit);
			friend vector_iterator operator - (const vector_iterator& vit, const difference_type i);
			friend difference_type operator - (const vector_iterator&, const vector_iterator&);
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

		//与容量相关
		difference_type size() const { return finish_ - start_; }
		difference_type capacity() const { return endOfStorage_ - start_; }
		bool empty() const { return start_ == finish_; }

		//访问元素相关
		value_type& operator[](const difference_type i) { return *(begin() + i); }
		value_type& front() { return *begin(); }
		value_type& back() { return *(--end()); }
		pointer data() { return start_; }

		//修改容器相关的操作
		void clear()
		{
			dataAllocator::destroy(start_, finish_);
			finish_ = start_;
		}

		void swap(vector& v)
		{
			if (this != &v)
			{
				std::swap(start_, v.start_);
				std::swap(finish_, v.finish_);
				std::swap(endOfStorage_, v.endOfStorage_);
			}
		}

		//TODO: push_back
		void push_back(const value_type& value);
		void pop_back()
		{
			--finish_;
			dataAllocator::destroy(finish_);
		}

		iterator insert(iterator position, const value_type& value);
		void insert(iterator position, size_type n, const value_type& value);
		template <typename It>
		void insert(iterator position, It first, It last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		Alloc get_allocator() { return dataAllocator; }
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

		template <typename InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type)
		{
			allocateAndCopy(first, last);
		}

		template <typename Integer>
		void vector_aux(Integer n, Integer value, std::true_type)
		{
			allocateAndFill(n, value);
		}
	};

	/*
		构造函数
	*/
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
		//处理指针和数字间区别的函数   vector 有两种实例化  v(begin, end) 和 v(n,x)
		vector_aux(begin, end, typename std::is_integral<It>::type());
	}

	/*
		拷贝构造函数
	*/
	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const vector& v)
	{
		allocateAndCopy(v.start_, v.finish_);
	}

	/*
		移动构造函数
	*/
	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(vector&& v)
	{
		start_ = v.start_;
		finish_ = v.finish_;
		endOfStorage_ = v.endOfStorage_;
		v.clear();
	}

	/*
		拷贝赋值运算符
	*/
	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v)
	{
		if (this != &v)
			allocateAndCopy(v.start_, v.finish_);
		return *this;
	}

	//------------------修改容器的相关操作------------------------
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
		//删去的对象数
		difference_type lenOfRemove = last - first;
		finish_ = finish_ - lenOfRemove;
		for (; lenOfTail != 0; --lenOfTail)
		{
			auto temp = last - lenOfRemove;
			*temp = *(last++); 
		}
		return vector_iterator<T>(first);
	}
}

#endif