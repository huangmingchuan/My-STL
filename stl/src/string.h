#ifndef _STRING_H_
#define _STRING_H_

#include "allocator.h"
#include "reverse_iterator.h"
#include "uninitialized_functions.h"

#include <cstring>
#include <type_traits>

namespace mystl
{
	class string
	{
	public:
		typedef char value_type;
		typedef char * iterator;
		typedef const char * const_iterator;
		typedef reverse_iterator_t<char*> reverse_iterator;
		typedef reverse_iterator_t<const char*> const_reverse_iterator;
		typedef char& reference;
		typedef const char& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		static const size_t npos = -1;

	private:
		char *start_;
		char *finish_;
		char *endOfStorage_;

		typedef mystl::allocator<char> dataAllocator;

	public:
		string() :start_(nullptr), finish_(nullptr), endOfStorage_(nullptr) {}
		string(const string& str);
		string(string&& str);
		string(const string& str, size_t pos, size_t len = npos);
		string(const char*);
		string(const char*, size_t n);
		string(size_t n, char c);
		template <typename InputIterator, typename InputIterator>
		string(InputIterator first, InputIterator last);

		string& operator=(const string& s);
		string& operator=(string&& s);
		string& operator=(const char* s);
		string& operator=(char c);

		~string();

		iterator begin() { return start_; }
		const_iterator begin() const { return start_; }
		iterator end() { return finish_; }
		const_iterator end() const { return finish_; }
		reverse_iterator rbegin() { return reverse_iterator(finish_); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(finish_); }
		reverse_iterator rend() { return reverse_iterator(start_); }
		const_reverse_iterator rend() const { return const_reverse_iterator(start_); }
		const_iterator cbegin() const { return start_; }
		const_iterator cend() const { return finish_; }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(finish_); }
		const_reverse_iterator crend() const { return const_reverse_iterator(start_); }
		size_t size() const { return finish_ - start_; }
		size_t length() const { return size(); }
		size_t capacity() const { return endOfStorage_ - finish_; }
		void clear()
		{
			dataAllocator::destroy(start_, finish_);
			finish_ = start_;
		}
		bool empty() const { return begin() == end(); }
		void resize(size_t n);
		void resize(size_t n, char c);
		void reserve(size_t = 0);
		void shrink_to_fit()
		{
			dataAllocator::deallocate(finish_, endOfStorage_ - finish_);
			endOfStorage_ = finish_;
		}

		char& operator[](size_t pos) { return *(start_ + pos); }
		const char& operator[](size_t pos) const { return *(start_ + pos); }
		char& back() { return *(finish_ - 1); }
		const char& back() const { return *(finish_ - 1); }
		char& front() { return *start_; }
		const char& front() const { return *start_; }

		string& insert(size_t pos, const string& str);
		string& insert(size_t pos, const string& str, size_t subpos, size_t sublen = npos);
		string& insert(size_t pos, const char* c);
		string& insert(size_t pos, const char* c, size_t n);
		string& insert(size_t pos, size_t n, char c);
		iterator insert(iterator p, size_t n, char c);
		iterator insert(iterator p, char c);
		template <typename InputIterator, typename InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);
		void push_back(char c) { insert(end(), c); }
		string& append(const string& str);
		string& append(const string& str, size_t subpos, size_t sublen = npos);
		string& append(const char* c);
		string& append(const char* c, size_t n);
		string& append(size_t n, char c);
		template <typename InputIterator>
		string& append(InputIterator first, InputIterator last);
		string& operator+=(const string& str);
		string& operator+=(const char* c);
		string& operator+=(char c);

		





	};
}


#endif