#pragma once
#include <cassert>
#include <iostream>
#include <stdexcept>
#include "Dalloc.hpp"

template<class T>
class Vector
{
private:

	Dalloc<T> _dAlloc = Dalloc<T>();
	T* _ptr;
	size_t _siz;
	size_t _cap;

	template<class X>
	class VectorItt
	{
		X* _ptr;

	public:

		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
		using iterator_category = std::random_access_iterator_tag;

		~VectorItt () = default;

		VectorItt () noexcept
		{
			_ptr = nullptr;
		}

		VectorItt (X* p) noexcept
		{
			_ptr = p; //Static cast?
		}

		VectorItt(const VectorItt& other) noexcept
		{
			_ptr = other._ptr; 
		}

		VectorItt& operator= (const VectorItt& other)
		{
			_ptr = other._ptr;
			return *this;
		}
		
		X& operator* () const
		{
			return *_ptr;
		}
		
		X* operator->() const
		{
			return _ptr;
		}
		
		X& operator[](size_t i) const noexcept
		{
			return _ptr[i];
		}

		VectorItt& operator++() noexcept
		{
			++_ptr;
			return *this;
		}
		
		VectorItt& operator--() noexcept
		{
			--_ptr;
			return *this;
		}
		
		VectorItt operator++(int) noexcept
		{
			auto iter = *this;
			operator++();
			return iter;
		}
		
		VectorItt operator--(int) noexcept
		{
			auto iter = *this;
			operator--();
			return iter;
		}

		VectorItt operator+=(difference_type i)  noexcept
		{
			_ptr = _ptr + i;
			return *this;
		}
		
		VectorItt operator+(difference_type i) const noexcept
		{
			return (_ptr + i);
		}
		
		VectorItt operator-(difference_type i) const noexcept
		{
			return (_ptr - i);
		}
		
		difference_type operator-(const VectorItt& other) const noexcept
		{
			return _ptr - other._ptr;
		}
		
		friend bool operator==(const VectorItt& lhs, const VectorItt rhs) noexcept { return lhs._ptr == rhs._ptr; }
		friend bool operator!=(const VectorItt& lhs, const VectorItt rhs) noexcept { return lhs._ptr != rhs._ptr; }
		friend bool operator<(const VectorItt& lhs, const VectorItt rhs) noexcept { return lhs._ptr < rhs._ptr; }
		friend bool operator>(const VectorItt& lhs, const VectorItt rhs) noexcept { return lhs._ptr > rhs._ptr; }
		friend bool operator<=(const VectorItt& lhs, const VectorItt rhs) noexcept { return !(lhs._ptr > rhs._ptr); }
		friend bool operator>=(const VectorItt& lhs, const VectorItt rhs) noexcept { return !(lhs._ptr < rhs._ptr); }
	};

public:

	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = VectorItt<T>;
	using const_iterator = VectorItt<const T>;
	using reverse_iterator = std::reverse_iterator<VectorItt<T>>;
	using const_reverse_iterator = std::reverse_iterator<VectorItt<const T>>;


	~Vector () noexcept //O(N)
	{
		for (int i = 0; i < _siz; i--)
		{
			_ptr[i].~T();
		}
		_dAlloc.deallocate(_ptr, _cap);
	}

	Vector () noexcept //O(1)
	{
		_siz = 0;
		_cap = 0;
		_ptr = nullptr; //Göra inits via kolon
	}

	Vector (const Vector& other) //O(N)
	{
		_siz = other._siz;
		_cap = other._cap;
		_ptr = _dAlloc.allocate(_cap);

		int offset = 0;
		try
		{
			while (offset < _siz)
			{
				new(_ptr + offset) T(other._ptr[offset]);
				offset++;
			}
		}
		catch (const std::exception&)
		{
			while (offset >= 0)
			{
				_ptr[offset].~T();
				offset--;
			}
			_dAlloc.deallocate(_ptr, _cap);
			throw;
		}
	}

	Vector (Vector&& other) noexcept //O(1)
	{
		_siz = other._siz;
		_cap = other._cap;
		_ptr = other._ptr;

		other._cap = 0;
		other._siz = 0;
		other._ptr = nullptr;
	}

	Vector (const char* other) //O(N)
	{

		_siz = strlen(other);
		_cap =  _siz * 2;
		_ptr = _dAlloc.allocate(_cap);

		int index = 0;
		try
		{
			while (other[index] != '\0')
			{
				new(_ptr + index) T(other[index]);
				index++;
			}
		}
		catch (const std::exception&)
		{
			while (index != 0)
			{
				_ptr[index].~T();
				index--;
			}
			throw;
		}


	}

	Vector& operator= (const Vector& other) //O(N)
	{
		Vector temp(other);
		swap(temp);
		return *this;
	}

	Vector& operator= (Vector&& other) noexcept //O(1)
	{

		if (this == &other) return *this;

		this->~Vector();
		
		_siz = other._siz;
		_cap = other._cap;
		_ptr = other._ptr;

		other._siz = 0;
		other._cap = 0;
		other._ptr = nullptr;

		return *this;
	}

	T& operator[] (size_t i) noexcept //O(1)
	{
		return _ptr[i];
	}

	const T& operator[] (size_t i) const noexcept //O(1)
	{
		return _ptr[i];
	}

	T& at (size_t i) //O(1)
	{
		if (!(i < _siz))
		{
			throw std::out_of_range("index out of range");
		}

		return _ptr[i];

	}

	const T& at (size_t i) const //O(1)
	{
		if (!(i < _siz))
		{
			throw std::out_of_range("index out of range");
		}

		return _ptr[i];
	}

	T* data () noexcept //O(1)
	{
		return _ptr;
	}

	const T* data() const noexcept //O(1)
	{
		return _ptr;
	}

	iterator begin() noexcept { return iterator(_ptr); } //O(1)
	iterator end () noexcept { return iterator(_ptr + _siz); } //O(1)
	const_iterator begin () const noexcept { return const_iterator(_ptr); } //O(1)
	const_iterator end () const noexcept { return const_iterator(_ptr + _siz); } //O(1)
	const_iterator cbegin () const noexcept { return const_iterator(_ptr); } //O(1)
	const_iterator cend () const noexcept { return const_iterator(_ptr + _siz); } //O(1)
	reverse_iterator rbegin () noexcept { return reverse_iterator(_ptr + _siz); } //O(1)
	reverse_iterator rend () noexcept { return reverse_iterator(_ptr); } //O(1)
	const_reverse_iterator rbegin () const noexcept { return const_reverse_iterator(_ptr + _siz); } //O(1)
	const_reverse_iterator rend () const noexcept { return const_reverse_iterator(_ptr); } //O(1)
	const_reverse_iterator crbegin () const noexcept { return const_reverse_iterator(_ptr + _siz); } //O(1)
	const_reverse_iterator crend () const noexcept { return const_reverse_iterator(_ptr); } //O(1)

	size_t size () const noexcept //O(1)
	{
		return _siz;
	}

	size_t capacity () const noexcept //O(1)
	{
		return _cap;
	}

	void reserve (size_t n) //O(N)
	{
		if (n > _cap)
		{
			if (_ptr != nullptr)
			{
				T* data = _dAlloc.allocate(n); //Allokera
				int index = 0;

				while (index < _siz)
				{
					new(data + index) T(std::move(_ptr[index])); //Ändrad till move, kan inte ge exceptions
					index++;
				}

				this->~Vector();
				_ptr = data;

			}
			else
			{
				_ptr = _dAlloc.allocate(n);
				_siz = 0;
			}
			_cap = n;
		}
	}

	void shrink_to_fit () //O(N)
	{
		if (_cap > _siz)
		{
			T* data = _dAlloc.allocate(_siz);
			int index = 0;
			while (index < _siz)
			{
				new(data + index) T(std::move(_ptr[index]));
				_ptr[index].~T();
				index++;
			}
			_dAlloc.deallocate(_ptr, _cap);
			_ptr = data;
			_cap = _siz;
		}
	}

	void push_back (const T& c) //O(1) amorterat
	{
		if (_siz == _cap)
		{
			if (_cap == 0)
			{
				reserve(8);
			}
			else
			{
				reserve(_cap * 2);
			}
		}
		try
		{
			new(_ptr + _siz) T(c);
			_siz++;
		}
		catch (const std::exception&)
		{
			throw;
		}
	}

	void push_back(T&& c) //O(1) amorterat
	{
		if (_siz == _cap)
		{
			if (_cap == 0)
			{
				reserve(8);
			}
			else
			{
				reserve(_cap * 2);
			}
		}
		try
		{
			new(_ptr + _siz) T(std::move(c));
			_siz++;
		}
		catch (const std::exception&)
		{
			throw;
		}
	}

	void resize (size_t n) //O(1) amoreterat
	{
		if (n > _cap) reserve(n);
		if (n > _siz)
		{
			for (size_t i = _siz; i < n; i++)
			{
				new(_ptr + i) T();
			}
		}
		_siz = n;
	}

	friend bool operator== (const Vector& lhs, const Vector& rhs) //O(N)
	{
		if (lhs.size() != rhs.size)
			return	false;
		auto iterLeft = lhs.begin();
		auto iterRght = rhs.begin();

		while (iterLeft != lhs.end() && iterRght != rhs.end())
		{
			if (*iterLeft != *iterRght) return false;
			++iterLeft;
			++iterRght;
		}
		return ( iterLeft == lhs.end() ) && ( iterRght == rhs.end() );
	}

	friend bool operator< (const Vector& lhs, const Vector& rhs)  //O(N)
	{
		auto iterLeft = lhs.begin();
		auto iterRght = rhs.begin();

		while (iterLeft != lhs.end() && iterRght != rhs.end())
		{
			if (*iterLeft != *iterRght) return *iterLeft < *iterRght;
			++iterLeft;
			++iterRght;
		}
		return (iterLeft == lhs.end()) && (iterRght != rhs.end());
	}

	friend bool operator!= (const Vector& lhs, const Vector& rhs) { return !(lhs == rhs); } //O(N)
	friend bool operator> (const Vector& lhs, const Vector& rhs) { return (rhs < lhs); } //O(N)
	friend bool operator<= (const Vector& lhs, const Vector& rhs) { return !(lhs > rhs); } //O(N)
	friend bool operator>= (const Vector& lhs, const Vector& rhs) { return !(lhs < rhs); } //O(N)

	bool Invariant() const
	{
		return (_siz <= _cap);
	}

	friend std::ostream& operator<< (std::ostream& cout, const Vector& other)
	{

		for (size_t i = 0; i < other.size(); ++i)
		{
			cout << other[i];
		}

		return cout;
	}

	friend void swap(Vector<T>& lhs, Vector<T>& rhs)
	{
		lhs.swap(rhs);
	}

	void swap(Vector& other)
	{
		std::swap(_ptr, other._ptr);
		std::swap(_siz, other._siz);
		std::swap(_cap, other._cap);
	}

#define CHECK assert(Invariant());

};