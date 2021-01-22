#pragma once
template<class T>
class List
{
	class Node;

	class Link
	{
		friend class List<T>;
		Link* _next;
		Link* _prev;
		Link() :_next(this), _prev(this) {}

		//iterator insert(iterator pos, const T& value);
		//iterator erase(const iterator& pos);
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
		/*
		ListIter(Node<T>* p) {};
		ListIter() {};
		ListIter(const ListIter& other) {};
		ListIter& operator=(const ListIter& other) {};
		T& operator*() {};
		T* operator->() {};
		ListIter& operator++() {};
		ListIter& operator--() {};
		ListIter operator++(int) {};
		ListIter operator--(int) {};
		friend bool operator == (const ListIter & lhs, const ListIter & rhs) {};
		friend bool operator != (const ListIter & lhs, const ListIter & rhs) {};
		*/
	};

	using iterator = ListIter<T>;

	Link _head;

public:
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

	List() 
	{
		_head._next = &_head;
		_head._prev = &_head;
	}

	List(const List& other)
	{
		_head._next = &_head;
		_head._prev = &_head;

		Link* tmp = other._head._next;

		while (tmp != other._head)
		{
			push_back(static_cast<Node*>(tmp)->_data);
			tmp = tmp->_next;
		}
	}

	List(const char* other)
	{
		_head._next = &_head;
		_head._prev = &_head;

		for (size_t i = 0; i < strlen(other); i++)
		{
			push_back(other[i]);
		}
	}

	T& front();

	T& back();

	const T& front() const;

	const T& back() const;

	iterator begin() noexcept;

	iterator end() noexcept;

	bool empty() const noexcept
	{
		//Probably wasted time to check both
		if (_head._next = &_head && _head._prev = &_head) return true;
		return false;
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

	void push_back(const T& value);
	void push_front(const T& value);
	void pop_back();
	void pop_front();
	friend bool operator==(const List& lhs, const List& other);
	friend bool operator<(const List& lhs, const List& other);
	friend std::ostream& operator<<(std::ostream& cout, const List& other);

	bool Invariant() 
	{
		int forwardCount = 0;
		int backwardsCount = 0;
		Link* tmp;

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

#define CHECK assert(Invariant());
};


