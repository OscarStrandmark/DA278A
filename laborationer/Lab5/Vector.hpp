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
		T* _ptr;

	public:

		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
		using iterator_category = std::random_access_iterator_tag;

		~VectorItt () = default;

		VectorItt () 
		{
			_ptr = nullptr;
		}

		VectorItt (T* p)
		{
			_ptr = p; //Static cast?
		}

		VectorItt(const VectorItt& other)
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
		
		X& operator[](size_t i)
		{
			return _ptr[i];
		}

		VectorItt& operator++()
		{
			++_ptr;
			return *this;
		}
		
		VectorItt& operator--()
		{
			--_ptr;
			return *this;
		}
		
		VectorItt operator++(int)
		{
			auto iter = *this;
			operator++();
			return iter;
		}
		
		VectorItt operator--(int)
		{
			auto iter = *this;
			operator--();
			return iter;
		}

		VectorItt operator+=(difference_type i) const
		{
			return _ptr + i;
		}

		VectorItt operator-=(difference_type i) const
		{
			return _ptr - i;
		}
		
		VectorItt operator+(difference_type i) const
		{
			return _ptr + i;
		}
		
		VectorItt operator-(difference_type i) const
		{
			return _ptr - i;
		}
		
		difference_type operator-(const VectorItt& other) const
		{
			return _ptr - other._ptr;
		}
		
		friend bool operator==(const VectorItt& lhs, const VectorItt rhs) { return lhs._ptr == rhs._ptr; }
		friend bool operator!=(const VectorItt& lhs, const VectorItt rhs) { return lhs._ptr != rhs._ptr; }
		friend bool operator<(const VectorItt& lhs, const VectorItt rhs) { return lhs._ptr < rhs._ptr; }
		friend bool operator>(const VectorItt& lhs, const VectorItt rhs) { return lhs._ptr > rhs._ptr; }
		friend bool operator<=(const VectorItt& lhs, const VectorItt rhs) { return !(lhs._ptr > rhs._ptr); }
		friend bool operator>=(const VectorItt& lhs, const VectorItt rhs) { return !(lhs._ptr < rhs._ptr); }
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
		_cap = 8;
		_ptr = _dAlloc.allocate(_cap);
	}

	Vector (const Vector& other) //O(N)
	{
		_siz = other._siz;
		_cap = other._cap;
		_ptr = _dAlloc.allocate(_cap);

		int offset = 0;
		try
		{
			while (offset <= _siz)
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

	Vector (const char* other) //O(N), Strong exception safety. Rollback if exception
	{

		_siz = 0; //Set to 0 here, gets incremented in push_back()
		_cap = strlen(other) * 2;
		_ptr = _dAlloc.allocate(_cap);

		int index = 0;
		try
		{
			while (other[index] != '\0')
			{
				push_back(other[index]);
				index++;
			}
		}
		catch (const std::exception&)
		{
			while (index >= 0)
			{
				_ptr[index].~T();
				index--;
			}
		}
	}

	Vector& operator= (const Vector& other) //O(N)
	{
		
		if (*this == other) return *this;

		reserve(other._siz);
		_siz = other._siz;

		for (size_t i = 0; i < _siz; i++)
		{
			_ptr[i] = other._ptr[i];
		}

		return *this;
	}

	Vector& operator= (Vector&& other) noexcept //O(1), kommer bli O(N)(?)
	{

		if (*this == other) return *this;

		_dAlloc.deallocate(_ptr, _cap);
		
		_siz = other._siz;
		_cap = other._cap;
		_ptr = other._ptr;

		other._siz = 0;
		other._cap = 0;
		other._ptr = nullptr;

		return *this;
	}

	T& operator[] (size_t i) //O(1)
	{
		return _ptr[i];
	}

	const T& operator[] (size_t i) const //O(1)
	{
		return _ptr[i];
	}

	T& at (size_t i) //O(1)
	{
		if (i >= _siz)
		{
			throw std::out_of_range("index out of range");
		}

		return _ptr[i];

	}

	const T& at (size_t i) const //O(1)
	{
		if (i >= _siz)
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
	reverse_iterator rbegin () noexcept { return reverse_iterator(_ptr); } //O(1)
	reverse_iterator rend () noexcept { return reverse_iterator(_ptr + _siz); } //O(1)
	const_reverse_iterator rbegin () const noexcept { return const_reverse_iterator(_ptr); } //O(1)
	const_reverse_iterator rend () const noexcept { return const_reverse_iterator(_ptr + _siz); } //O(1)
	const_reverse_iterator rcbegin () const noexcept { return const_reverse_iterator(_ptr); } //O(1)
	const_reverse_iterator rcend () const noexcept { return const_reverse_iterator(_ptr + _siz); } //O(1)

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
			T* data = _dAlloc.allocate(n);
			for (size_t i = 0; i < _siz; i++)
			{
				data[i] = _ptr[i];
			}
			
			_dAlloc.deallocate(_ptr, _cap);
			_ptr = data;
			_cap = n;
		}
	}

	void shrink_to_fit () //O(N)
	{
		if (_cap > _siz)
		{
			T* data = _dAlloc.allocate(_siz);
			for(size_t i = 0; i < _siz; i++)
			{
				data[i] = _ptr[i];
			}

			_dAlloc.deallocate(_ptr, _cap);
			_ptr = data;
			_cap = _siz;
		}
	}

	void push_back (T c) //O(1) amorterat
	{
		//Kontrollera att _cap != 0
		if (_siz == _cap) reserve(_cap * 2);
		_ptr[_siz++] = c;
	}

	void resize (size_t n) //O(1) amoreterat
	{
		if (n > _cap) reserve(n);
		if (n > _siz)
		{
			for (size_t i = _siz; i < n; i++)
			{
				_ptr[i] = T();
			}
		}
		_siz = n;
	}

	friend bool operator== (const Vector& lhs, const Vector& rhs) //O(N)
	{
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
		return (_ptr != nullptr) && (_siz <= _cap);
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