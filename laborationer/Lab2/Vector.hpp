#pragma once
#include <cassert>

template<class T>
class Vector
{
private:

	T* _ptr;
	size_t _size;
	size_t _cap;

public:

	Vector() noexcept
	{
		_size = 0;
		_cap = 8;
		_ptr = new T[_cap];
	}

	~Vector()
	{
		delete[] _ptr;
	}

	Vector(const Vector& other)
	{
		_size = other._size;
		_cap = other._cap;
		_ptr = new T[_cap];

		for (size_t i = 0; i < _size; i++)
		{
			_ptr[i] = other._ptr[i];
		}
	}

	Vector(Vector&& other)
	{
		_size = other._size;
		_cap = other._cap;
		_ptr = other._ptr;

		other._cap = 0;
		other._size = 0;
		other._ptr = nullptr;
	}

	using iterator = VectorItt<T>;

	class VectorItt
	{

	};

	size_t size() const noexcept
	{
		return _size;
	}

	bool Invariant() const
	{

	}

#define CHECK assert(Invariant());

};

