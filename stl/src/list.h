#ifndef _LIST_H_
#define _LIST_H_

#include "iterator.h"
#include "reverse_iterator.h"
#include "uninitialized_functions.h"
#include "allocator.h"

#include <type_traits>

namespace mystl
{
	template <typename T>
	class list;

	template <typename T>
	struct node
	{
		T data;
		node *prev;
		node *next;
		list<T> *container;
		node(const T& d, node *p, node *n, list<T> *c) :data(d), prev(p), next(n), container(c) {}

		bool operator==(const node& n)
		{
			return data == n.data && prev == n.prev && next == n.next && container == n.container;
		}
	};

	template <typename T>
	struct list_iterator : public iterator<bidirectional_iterator_tag, T>
	{
		template <typename T>
		friend class list;

		typedef node<T>* nodePtr;
		nodePtr p;

		explicit list_iterator(nodePtr ptr = nullptr) : p(ptr) {}
		
		list_iterator& operator++();
		list_iterator operator++(int);
		list_iterator& operator--();
		list_iterator operator--(int);
		T& operator*() { return p->data; }
		T* operator->() { return &(operator*()); }

		template <typename T>
		friend bool operator==(const list_iterator<T>& lhs, const list_iterator<T>& rhs);
		template <typename T>
		friend bool operator!=(const list_iterator<T>& lhs, const list_iterator<T>& rhs);
	};

	template <typename T>
	class list
	{
		template <typename T>
		friend struct list_iterator;

	private:
		typedef allocator<node<T>> nodeAllocator;
		typedef node<T>* nodePtr;

	public:
		typedef T value_type;
		typedef list_iterator<T> iterator;
		typedef list_iterator<const T> const_iterator;
		typedef reverse_iterator_t<iterator> reverse_iterator;
		typedef T& reference;
		typedef size_t size_type;

	private:
		iterator head;
		iterator tail;
		
	public:
		list();
		explicit list(size_type n, const value_type& val = value_type());
		template <typename InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& l);
		list& operator=(const list& l);
		~list();
	};
}

#endif