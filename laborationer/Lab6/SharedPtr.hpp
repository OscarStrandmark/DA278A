#pragma once
#include <cstddef>
#include <iostream>
#include <functional>
#include <algorithm>

template<class T>
class SharedPtr
{
	T* _ptr;
	int* _refCount;

	void _addReference(T* ptr)
	{
		if (ptr != nullptr)
		{
			if (_refCount == nullptr)
			{
				_refCount = new int(1);
			}
			else
			{
				++*_refCount;
			}
		}
	}

	void _removeReference()
	{
		if (_refCount != nullptr)
		{
			--*_refCount;
			if (*_refCount == 0)
			{
				delete _ptr;
				delete _refCount;
			}
			_refCount = nullptr;
		}
		_ptr = nullptr;
	}
public:

	~SharedPtr()
	{
		_removeReference();
	}

	SharedPtr()
	{
		_ptr = nullptr;
		_refCount = nullptr;
	}

	SharedPtr(std::nullptr_t nullp)
	{
		_ptr = nullptr;
		_refCount = nullptr;
	}

	SharedPtr(T* other)
	{
		_ptr = other;
		_refCount = nullptr;
		_addReference(other);
	}

	SharedPtr(SharedPtr& other)
	{
		if (this == &other)
			return;

		_ptr = other._ptr;
		_refCount = other._refCount;
		_addReference(_ptr);
	}

	SharedPtr(SharedPtr&& other)
	{
		if (this == &other)
			return;

		_ptr = other._ptr;
		_refCount = other._refCount;
		_addReference(_ptr);
		other._removeReference();
	}

	SharedPtr& operator=(SharedPtr& other)
	{
		if (this == &other)
			return *this;

		_removeReference();
		_ptr = other._ptr;
		_refCount = other._refCount;
		_addReference(_ptr);

		return *this;
	}

	SharedPtr& operator=(SharedPtr&& other)
	{
		if (this == &other)
			return *this;

		_removeReference();
		_ptr = other._ptr;
		_refCount = other._refCount;
		_addReference(_ptr);
		other._removeReference();

		return *this;
	}

	SharedPtr& operator=(std::nullptr_t nullp)
	{
		if (_ptr == nullptr)
			return *this;

		_removeReference();

		return *this;
	}

	T& operator*() const noexcept
	{
		return *_ptr;
	}

	T* operator->() const noexcept
	{
		return _ptr;
	}

	explicit operator bool() const noexcept
	{
		return (_ptr != nullptr);
	}

	void reset() noexcept
	{
		_removeReference();
	}

	T* get() const noexcept
	{
		return _ptr;
	}

	long use_count() const noexcept
	{
		return *_refCount;
	}


	bool Invariant() const noexcept
	{
		if (_refCount == nullptr && _ptr == nullptr)
			return true;
		else if (_refCount != nullptr && _ptr != nullptr)
			return true;
		return false;
	}

	bool operator== (const SharedPtr<T>& rhs) const
	{ 
		return get() == rhs.get(); 
	}

	bool operator!= (const SharedPtr<T>& rhs) const
	{
		return !(this == rhs); 
	}

	bool operator<  (const SharedPtr<T>& rhs) const
	{
		return std::less<T*>()(get(), rhs.get()); 
	}

	bool operator>  (const SharedPtr<T>& rhs) const
	{
		return rhs < *this;
	}

	bool operator<= (const SharedPtr<T>& rhs) const
	{
		return !(rhs < *this);
	}
	
	bool operator>= (const SharedPtr<T>& rhs) const
	{
		return !(*this < rhs);
	}

	bool operator== (const std::nullptr_t nptr) const
	{
		return !operator bool();
	}

	bool operator!= (const std::nullptr_t nptr) const
	{
		return operator bool();
	}

	bool operator<  (const std::nullptr_t nptr) const
	{
		return std::less<T*>()(get(), nptr);
	}

	bool operator>  (const std::nullptr_t nptr) const
	{
		return nptr < *this;
	}

	bool operator<= (const std::nullptr_t nptr) const
	{
		return !(nptr < *this);
	}

	bool operator>= (const std::nullptr_t nptr) const
	{
		return !(*this < nptr);
	}
};

template<class T>
void swap(SharedPtr<T>& ptr1, SharedPtr<T>& ptr2)
{
	 std::swap(ptr1, ptr2);
}

template<class T, class ... Args>
SharedPtr<T> MakeShared(Args&&... args)
{
	SharedPtr<T> temp(new T(args...));
	return temp;
}