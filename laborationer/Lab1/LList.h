#pragma once

template<class T>
class LList
{
	
	class Link
	{
		friend class List<T>;
		Link* _next;
		Link* _prev;
		Link() :_next(this), _prev(this);

		iterator insert(iterator pos, const T& value);
		iterator erase(const iterator& pos);
	};

	class Node : public Link
	{
		friend class List;
		T _data;
	public:
		Node(const T& data) :_data(data) {};
	};

	template<class X>
	class LListIterator
	{
		friend class List;
		Link* _ptr;
	public:
		typedef iterator_category;
		typedef value_type;
		typedef difference_type;
		typedef reference;
		typedef pointer;
		LListIterator(Node<T>* p);
		LListIterator();
		LListIterator(const LListIterator& other);
		T& operator*();
		T* operator->();
		LListIterator& operator++();
		LListIterator& operator--();
		LListIterator operator++(int);
		LListIterator operator--(int);
		friend bool operator==(const LListIterator& lhs, const LListIterator& rhs);
		friend bool operator!=(const LListIterator& lhs, const LListIterator& rhs);
	};



};

