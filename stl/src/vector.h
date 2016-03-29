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


	}
}

#endif