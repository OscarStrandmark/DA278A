#pragma once

#define CHECK assert(Invariant());
#include <iostream>
#include <cassert>

template<class T>
class List
{
	class Node;

	class Link
	{
		friend class List<T>;
		Link* _next;
		Link* _prev;
		Link() :_next(this), _prev(this) {};

		//Insert node after this 
		void insert(Node* node) 
		{
			node->_next = this;
			node->_prev = _prev;
			_prev->_next = node;
			this->_prev = node;
		}

		//erase this
		void erase()
		{
			_prev->_next = _next;
			_next->_prev = _prev;
			delete static_cast<Node*>(this);
		}	
	};

	class Node : public Link {
		friend class List;
		T _data;
	public:
		Node(const T& data) : _data(data) {};
	};

	template<class X>
	class ListIter
	{
		friend class List;
		Link* _ptr;
	public:

		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;

		~ListIter() = default;

		ListIter()
		{
			_ptr = nullptr;
		}

		ListIter(const ListIter& other) 
		{
			_ptr = other._ptr;
		}

		ListIter(Link* ptr) : _ptr(static_cast<Node*>(ptr)) {}

		T& operator*() 
		{ 
			return static_cast<Node*>(_ptr)->_data; 
		}

		T* operator->() 
		{
			return &static_cast<Node*>(_ptr)->_data;
		}

		ListIter& operator++()  //pre
		{
			_ptr = _ptr->_next;
			return *this;
		}

		ListIter& operator--() //pre
		{
			_ptr = _ptr->_prev;
			return *this;
		}

		ListIter operator++(int) //post
		{
			ListIter retVal = *this;
			operator++();
			return retVal;
		}

		ListIter operator--(int) //post
		{
			ListIter retVal = *this;
			operator--();
			return retVal;
		}

		friend bool operator == (const ListIter & lhs, const ListIter & rhs) 
		{
			return lhs._ptr == rhs._ptr;
		}

		friend bool operator != (const ListIter & lhs, const ListIter & rhs) 
		{
			return lhs._ptr != rhs._ptr;
		}
	};

	Link _head;

public:

	using iterator = ListIter<T>;
	using const_iterator = ListIter<const T>;

	~List()
	{
		if (size() != 0)
		{
			Link* tmp = _head._next;

			while (tmp != &_head)
			{
				tmp = tmp->_next;
				delete tmp->_prev;
			}
		}
	}

	List() = default;

	//Vänsterlänkar vid skapande, höger sist

	List(const List& other)
	{
		for (auto iter = other.begin(); iter != other.end(); ++iter)
		{
			push_back(*iter);
		}
	}

	List(const char* other)
	{
		const char* p = other;
		while (*p != '\0') //c-strings null terminated
		{
			push_back(*p);
			++p;
		}
	}


	iterator insert(iterator pos, const T& value) //Add item at pos, return iterator on pos
	{
		Node* node = new Node(value);
		pos._ptr->insert(node);
		return iterator(node);
	}

	iterator erase(const iterator& pos) //Erase item at pos, return iterator to next element
	{
		Link* next = pos._ptr->_next;
		pos._ptr->erase();
		return iterator(next);
	}

	T& front() //Get ref to first item
	{
		return *begin();
	}

	T& back() //Get ref to last item
	{
		iterator iter = end();
		--iter;
		return *iter;
	}

	const T& front() const 
	{
		return *cbegin();
	}

	const T& back() const 
	{
		const_iterator iter = cend();
		--iter;
		return *iter;
	}

	iterator begin() noexcept { return iterator(_head._next); }
	const_iterator begin() const noexcept { return const_iterator(const_cast<Link*>(_head._next)); }
	const_iterator cbegin() const noexcept { return const_iterator(const_cast<Link*>(_head._next)); }

	iterator end() noexcept { return iterator(&_head); } //End iterators give after last element, so head
	const_iterator end() const noexcept { return const_iterator(const_cast<Link*>(&_head)); }
	const_iterator cend() const noexcept { return const_iterator(const_cast<Link*>(&_head)); }

	bool empty() const noexcept
	{
		return begin() == end();
	}

	size_t size() const noexcept
	{
		size_t count = 0;

		Link* tmp = _head._next;
		while (tmp != &_head)
		{
			count++;
			tmp = tmp->_next;
		}

		return count;
	}

	int Count()
	{
		int count = 0;

		Link* tmp = _head._next;
		while (tmp != &_head)
		{
			count++;
			tmp = tmp->_next;
		}

		return count;
	}

	void Print(std::ostream& cout)
	{
		Link* tmp = _head._next;
		while (tmp != &_head)
		{
			cout << static_cast<Node*>(tmp)->_data;
			tmp = tmp->_next;
		}
	}

	void push_back(const T& value) 
	{
		insert(end(), value);
	}

	void push_front(const T& value)
	{
		insert(begin(), value);
	}

	void pop_back() 
	{
		erase(end()._ptr->_prev);
	}

	void pop_front()
	{
		erase(begin()._ptr);
	}

	friend bool operator==(const List& lhs, const List& rhs) 
	{
		auto iterLeft = lhs.begin();
		auto iterRght = rhs.begin();

		while (iterLeft != lhs.end() && iterRght != rhs.end())
		{
			if (*iterLeft != *iterRght) return false;
			++iterLeft;
			++iterRght;
		}

		return iterLeft == lhs.end() && iterRght == rhs.end();
	}

	friend bool operator<(const List& lhs, const List& rhs) 
	{
		auto iterLeft = lhs.begin();
		auto iterRght = rhs.begin();

		while (iterLeft != lhs.end() && iterRght != rhs.end())
		{
			if (*iterLeft != *iterRght) return *iterLeft < *iterRght;
			++iterLeft;
			++iterRght;
		}
		return iterLeft == lhs.end() && iterRght != rhs.end();
	}

	friend bool operator!=(const List& lhs, const List& rhs) { return !(lhs == rhs); };
	friend bool operator>(const List& lhs, const List& rhs) { return (rhs < lhs); };
	friend bool operator<=(const List& lhs, const List& rhs) { return !(rhs < lhs); };
	friend bool operator>=(const List& lhs, const List& rhs) { return !(lhs < rhs); };
	
	friend std::ostream& operator<<(std::ostream& cout, const List& rhs) 
	{
		for (auto it = rhs.begin(); it != rhs.end(); ++it)
		{
			cout << *it << " "; cout << std::endl;
		}
		return cout;
	};

	bool Invariant() 
	{
		int forwardCount = 0;
		int backwardsCount = 0;
		Link* tmp;

		//Bättre att ramla igenom en gång och kolla att tmp->next->prev

		tmp = _head._next;
		while (tmp != &_head)
		{
			forwardCount++;
			tmp = tmp->_next;
		}

		tmp = _head._prev;
		while (tmp != &_head)
		{
			backwardsCount++;
			tmp = tmp->_prev;
		}

		if (forwardCount == Count() && backwardsCount == Count()) return true;
		return false;
	}
};