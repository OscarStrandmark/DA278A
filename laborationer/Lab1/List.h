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

		iterator insert(iterator pos, const T& value);
		iterator erase(const iterator& pos);
	};

	class Node : public Link {
		friend class List;
		T _data;
	public:
		Node(const T& data) : _data(data) {};
	};
};