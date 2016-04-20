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

		bool empty() const { return head == tail; }
		size_type size() const;
		reference front() { return head.p->data; }
		reference back() { return tail.p->prev->data; }

		void push_front(const value_type& val);
		void pop_front();
		void push_back(const value_type& val);
		void pop_back();

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		reverse_iterator rbegin();
		reverse_iterator rend();

		iterator insert(iterator position, const value_type& val);
		void insert(iterator position, size_type n, const value_type& val);
		template <typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(list& x);
		void clear();
		void splice(iterator position, list& x);
		void splice(iterator position, list& x, iterator i);
		void splice(iterator position, list& x, iterator first, iterator last);
		void remove(const value_type& val);
		template <typename Predicate>
		void remove_if(Predicate pre);
		void unique();
		template <typename BinaryPredicate>
		void unique(BinaryPredicate p);
		void merge(list& x);
		template <typename Compare>
		void merge(list& x, Compare com);
		void sort();
		template <typename Compare>
		void sort(Compare com);
		void reverse();

	private:
		void ctorAux(size_type n, const value_type& val, std::true_type);
		template <typename InputIterator>
		void ctorAux(InputIterator first, InputIterator last, std::false_type);
		nodePtr newNode(const T& val = T());
		void deleteNode(nodePtr p);
		void insert_aux(iterator position, size_type n, const T& val, std::true_type);
		template <typename InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		const_iterator changeIteratorToConstIterator(iterator& it) const;

	public:
		template <typename T>
		friend void swap(list<T>& x, list<T>& y);
		template <typename T>
		friend bool operator==(const list<T>& lhs, const list<T>& rhs);
		template <typename T>
		friend bool operator!=(const list<T>& lhs, const list<T>& rhs);
	};

	template <typename T>
	list_iterator<T>& list_iterator<T>::operator++()
	{
		p = p->next;
		return *this;
	}

	template <typename T>
	list_iterator<T> list_iterator<T>::operator++(int)
	{
		auto res = *this;
		++*this;
		return res;
	}

	template <typename T>
	list_iterator<T>& list_iterator<T>::operator--()
	{
		p = p->prev;
		return *this;
	}

	template <typename T>
	list_iterator<T> list_iterator<T>::operator--(int)
	{
		auto res = *this;
		--*this;
		return res;
	}

	template <typename T>
	bool operator==(const list_iterator<T>& lhs, const list_iterator<T>& rhs)
	{
		return lhs.p == rhs.p;
	}

	template <typename T>
	bool operator!=(const list_iterator<T>& lhs, const list_iterator<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T>
	void list<T>::insert_aux(iterator position, size_type n, const T& val, std::true_type)
	{
		for (auto i = n; i != 0; --i)
		{
			position = insert(position, val);
		}
	}

	template <typename T>
	template <typename InputIterator>
	void list<T>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type)
	{
		for (--last; first != last; --last)
		{
			position = insert(position, *last);
		}
		insert(position, *last);
	}

	template <typename T>
	typename list<T>::nodePtr list<T>::newNode(const T& val = T())
	{
		nodePtr res = nodeAllocator::allocate();
		nodeAllocator::construct(res, node<T>(val, nullptr, nullptr, this));
		return res;
	}

	template <typename T>
	void list<T>::deleteNode(nodePtr p)
	{
		p->next = p->prev = nullptr;
		nodeAllocator::destroy(p);
		nodeAllocator::deallocate(p);
	}

	template <typename T>
	void list<T>::ctorAux(size_type n, const T& val, std::true_type)
	{
		head.p = newNode();
		tail.p = head.p;
		while (n--)
		{
			push_back(val);
		}
	}

	template <typename T>
	template <typename InputIterator>
	void list<T>::ctorAux(InputIterator first, InputIterator last, std::false_type)
	{
		head.p = newNode();
		tail.p = head.p;
		for (; first != last; ++first)
		{
			push_back(*first);
		}
	}

	template <typename T>
	typename list<T>::size_type list<T>::size() const
	{
		size_type len = 0;
		for (auto h = head; h != tail; ++h)
		{
			++len;
		}
		return len;
	}

	template <typename T>
	list<T>::list()
	{
		head.p = newNode();
		tail.p = head.p;
	}

	template <typename T>
	list<T>::list(size_type n, const value_type& val = value_type())
	{
		ctorAux(n, val, typename std::is_integral<value_type>::type());
	}

	template <typename T>
	template <typename InputIterator>
	list<T>::list(InputIterator first, InputIterator last)
	{
		ctorAux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template <typename T>
	list<T>::list(const list& l)
	{
		head.p = newNode();
		tail.p = head.p;
		for (auto node = l.head.p; node != l.tail.p; node = node->next)
		{
			push_back(node->data);
		}
	}

	template <typename T>
	list<T>& list<T>::operator=(const list& l)
	{
		if (this != &l)
		{
			list(l).swap(*this);
		}
		return *this;
	}

	template <typename T>
	list<T>::~list()
	{
		while (head != tail)
		{
			auto temp = head++;
			nodeAllocator::destroy(temp.p);
			nodeAllocator::deallocate(temp.p);
		}
		nodeAllocator::deallocate(tail.p);
	}

	template <typename T>
	void list<T>::push_front(const value_type& val)
	{
		auto node = newNode(val);
		head.p->prev = node;
		node->next = head.p;
		head.p = node;
	}

	template <typename T>
	void list<T>::pop_front()
	{
		auto oldNode = head.p;
		head.p = oldNode->next;
		head.p->prev = nullptr;
		deleteNode(oldNode);
	}

	template <typename T>
	void list<T>::push_back(const value_type& val)
	{
		auto node = newNode();
		tail.p->data = val;
		tail.p->next = node;
		node->prev = tail.p;
		tail.p = node;
	}

	template <typename T>
	void list<T>::pop_back()
	{
		auto newTail = tail.p->prev;
		newTail->next = nullptr;
		deleteNode(tail.p);
		tail.p = newTail;
	}

	template <typename T>
	typename list<T>::iterator list<T>::insert(iterator position, const value_type& val)
	{
		if (position == begin())
		{
			push_front(val);
			return begin();
		}
		else if (position == end())
		{
			push_back(val);
			return --end();
		}
		auto node = newNode(val);
		auto prev = position.p->prev;
		node->next = position.p;
		node->prev = prev;
		prev->next = node;
		position.p->prev = node;
		return iterator(node);
	}

	template <typename T>
	void list<T>::insert(iterator position, size_type n, const value_type& val)
	{
		insert_aux(position, n, val, typename std::is_integral<value_type>::type());
	}

	template <typename T>
	template <typename InputIterator>
	void list<T>::insert(iterator position, InputIterator first, InputIterator last)
	{
		insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
	}

	template <typename T>
	typename list<T>::iterator list<T>::erase(iterator position)
	{
		if (position == head)
		{
			pop_front();
			return head;
		}
		else
		{
			auto prev = position.p->prev;
			prev->next = position.p->next;
			position.p->next->prev = prev;
			deleteNode(position.p);
			return iterator(prev->next);
		}
	}	

	template <typename T>
	typename list<T>::iterator list<T>::erase(iterator first, iterator last)
	{
		typename list<T>::iterator res;
		while (first != last)
		{
			auto temp = first++;
			res = erase(temp);
		}
		return res;
	}

	template <typename T>
	void list<T>::clear()
	{
		erase(begin(), end());
	}

	template <typename T>
	typename list<T>::iterator list<T>::begin()
	{
		return head;
	}

	template <typename T>
	typename list<T>::iterator list<T>::end()
	{
		return tail;
	}

	template <typename T>
	typename list<T>::const_iterator list<T>::changeIteratorToConstIterator(iterator& it) const
	{
		using nodeP = node<const T>*;
		auto temp = (list<const T>*const)this;
		auto ptr = it.p;
		node<const T> node(ptr->data, (nodeP)(ptr->prev), (nodeP)(ptr->next), temp);
		return const_iterator(&node);
	}

	template <typename T>
	typename list<T>::const_iterator list<T>::begin() const
	{
		auto temp = (list*const)this;
		return changeIteratorToConstIterator(temp->head);
	}

	template <typename T>
	typename list<T>::const_iterator list<T>::end() const
	{
		auto temp = (list*const)this;
		return changeIteratorToConstIterator(temp->tail);
	}

	template <typename T>
	typename list<T>::reverse_iterator list<T>::rbegin()
	{
		return reverse_iterator(tail);
	}

	template <typename T>
	typename list<T>::reverse_iterator list<T>::rend()
	{
		return reverse_iterator(head);
	}

	template <typename T>
	void list<T>::reverse()
	{
		if (empty() || head.p->next == tail.p) return;
		auto curNode = head.p;
		head.p = tail.p->prev;
		head.p->prev = nullptr;
		do
		{
			auto nextNode = curNode->next;
			curNode->next = head.p->next;
			head.p->next->prev = curNode;
			curNode->prev = head.p;
			head.p->next = curNode;
			curNode = nextNode;
		} while (curNode != head.p);
	}

	template <typename T>
	void list<T>::remove(const value_type& val)
	{
		for (auto it = begin(); it != end();)
		{
			if (*it == val)
				it = erase(it);
			else
				++it;
		}
	}

	template <typename T>
	template <typename Predicate>
	void list<T>::remove_if(Predicate pre)
	{
		for (auto it = begin(); it != end();)
		{
			if (pre(*it))
				it = erase(it);
			else
				++it;
		}
	}

	template <typename T>
	void list<T>::swap(list& x)
	{
		std::swap(head.p, x.head.p);
		std::swap(tail.p, x.tail.p);
	}

	template <typename T>
	void swap(list<T>& x, list<T>& y)
	{
		x.swap(y);
	}

	template <typename T>
	void list<T>::unique()
	{
		nodePtr curNode = head.p;
		while (curNode != tail.p)
		{
			nodePtr nextNode = curNode->next;
			if (curNode->data == nextNode->data)
			{
				if (nextNode == tail.p)
				{
					curNode->next == nullptr;
					tail.p = curNode;
				}
				else
				{
					curNode->next = nextNode->next;
					nextNode->next->prev = curNode;
				}
				deleteNode(nextNode);
			}
			else
			{
				curNode = nextNode;
			}
		}
	}

	template <typename T>
	template <typename BinaryPredicate>
	void list<T>::unique(BinaryPredicate pre)
	{
		nodePtr curNode = head.p;
		while (curNode != tail.p)
		{
			nodePtr nextNode = curNode->next;
			if (pre(curNode->data,nextNode->data))
			{
				if (nextNode == tail.p)
				{
					curNode->next == nullptr;
					tail.p = curNode;
				}
				else
				{
					curNode->next = nextNode->next;
					nextNode->next->prev = curNode;
				}
				deleteNode(nextNode);
			}
			else
			{
				curNode = nextNode;
			}
		}
	}

	template <typename T>
	void list<T>::splice(iterator position, list& x)
	{
		this->insert(position, x.begin(), x.end());
		x.head.p = x.tail.p;
	}

	template <typename T>
	void list<T>::splice(iterator position, list& x, iterator first, iterator last)
	{
		if (first.p == last.p) return;
		auto tailNode = last.p->prev;
		if (x.head.p == first.p)
		{
			x.head.p = last.p;
			x.head.p->prev = nullptr;
		}
		else
		{
			first.p->prev->next = last.p;
			last.p->prev = first.p->prev;
		}
		if (position.p == head.p)
		{
			first.p->prev = nullptr;
			tailNode->next = head.p;
			head.p->prev = tailNode;
			head.p = first.p;
		}
		else
		{
			position.p->prev->next = first.p;
			first.p->prev = position.p->prev;
			tailNode->next = position.p;
			position.p->prev = tailNode;
		}
	}

	template <typename T>
	void list<T>::splice(iterator position, list& x, iterator i)
	{
		auto next = i;
		this->splice(position, x, i, ++next);
	}

	template <typename T>
	void list<T>::merge(list& x)
	{
		auto it1 = begin(), it2 = x.begin();
		while (it1 != end() && it2 != x.end())
		{
			if (*it1 <= *it2)
				++it1;
			else
			{
				auto temp = it2++;
				this->splice(it1, x, temp);
			}
		}
		if (it1 == end())
			this->splice(it1, x, it2, x.end());
	}

	template <typename T>
	template <typename Compare>
	void list<T>::merge(list& x, Compare comp)
	{
		auto it1 = begin(), it2 = x.begin();
		while (it1 != end() && it2 != x.end())
		{
			if (comp(*it2, *it1))
			{
				auto temp = it2++;
				this->splice(it1, x, temp);
			}
			else
				++it1;
		}
		if (it1 == end())
			this->splice(it1, x, it2, x.end());
	}

	 template <typename T>
	 bool operator==(const list<T>& lhs, const list<T>& rhs)
	 {
		 auto node1 = lhs.head.p, node2 = rhs.head.p;
		 for (; node1 != lhs.tail.p && node2 != rhs.tail.p; node1 = node1->next, node2 = node2->next)
		 {
			 if (node1->data != node2->data)
				 return false;
		 }
		 return (node1 == lhs.tail.p && node2 == rhs.tail.p);
	 }

	 template <typename T>
	 bool operator!=(const list<T>& lhs, const list<T>& rhs)
	 {
		 return !(lhs == rhs);
	 }

	 template <typename T>
	 void list<T>::sort()
	 {
		 sort(std::less<T>());
	 }

	 //自底向上归并排序
	 template <typename T>
	 template <typename Compare>
	 void list<T>::sort(Compare comp)
	 {
		 if (empty() || head.p->next == tail.p)
			 return;
		 list carry;
		 list counter[64];
		 int fill = 0;
		 while (!empty())
		 {
			 carry.splice(carry.begin(), *this, begin());
			 int i = 0;
			 while (i < fill && !counter[i].empty())
			 {
				 counter[i].merge(carry, comp);
				 carry.swap(counter[i++]);
			 }
			 carry.swap(counter[i]);
			 if (i == fill)
				 ++fill;
		 }
		 for (int i = 0; i != fill; ++i)
			 counter[i].merge(counter[i - 1], comp);

		 swap(counter[fill - 1]);
	 }
}

#endif