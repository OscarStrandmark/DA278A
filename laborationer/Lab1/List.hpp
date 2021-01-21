#pragma once
template<class T>
class List
{
	template<class T>
	class Node;

	template<class T>
	class Link
	{
		friend class List<T>;
		Link* _next;
		Link* _prev;
		Link() :_next(this), _prev(this) {}

		iterator insert(iterator pos, const T& value);
		iterator erase(const iterator& pos);
	};

	template<class T>
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
		ListIter(Node<T>* p);
		ListIter();
		ListIter(constListIter& other);
		ListIter& operator=(constListIter& other);
		T& operator*();
		T* operator->();
		ListIter& operator++();
		ListIter& operator--();
		ListIter operator++(int);
		ListIter operator--(int);
		friend bool operator == (constListIter & lhs, constListIter & rhs);
		friend bool operator != (constListIter & lhs, constListIter & rhs);
	};

	using iterator = ListIter<T>;

	Link _head;

	~List();
	List();
	List(const List& other);
	List(const char* other);
	T& front();
	T& back();
	const T& front() const;
	const T& back() const;
	iterator begin() noexcept;
	iterator end() noexcept;
	bool empty() const noexcept;
	size_t size() const noexcept;
	void push_back(const T& value);
	void push_front(const T& value);
	void pop_back();
	void pop_front();
	friend bool operator==(const List& lhs, const List& other);
	friend bool operator<(const List& lhs, const List& other);
	friend std::ostream& operator<<(std::ostream& cout, const List& other);
	bool Invariant();
#define CHECK assert(Invariant());
};

