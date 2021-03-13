#pragma once
#include <cstddef>
#include <iostream>

template<class T>
class SharedPtr
{
	T* _ptr;
	int* _refCount;

	void addRef(T* ptr)
	{
		if (ptr != nullptr)
		{
			if (_refCount == nullptr)
			{
				_refCount = new int(1);
			}
			else
			{
				*_refCount++;
			}
		}
	}

	void delRef()
	{
		if (_refCount != nullptr)
		{
			*_refCount--;
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
		delRef();
	}

	SharedPtr()
	{
		_ptr = nullptr;
		_refCount = nullptr;
	}

	SharedPtr(T* other)
	{
		_ptr = other;
		_refCount = nullptr;
		addRef(other);
	}

	SharedPtr(SharedPtr& other)
	{
		if (this == &other)
			return;

		_ptr = other._ptr;
		_refCount = other._refCount;
		addRef(_ptr);
	}

	SharedPtr(SharedPtr&& other)
	{
		if (this == &other)
			return;
		_ptr = other._ptr;
		_refCount = other._refCount;
		addRef(_ptr);
		other.delRef();
	}

	//SharedPtr(std::nullptr);

	SharedPtr& operator=(SharedPtr& other)
	{
		if (this == &other)
			return *this;

		delRef();
		_ptr = other._ptr;
		_refCount = other._refCount;
		addRef(_ptr);

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
		delRef();
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

	bool operator==(SharedPtr& other);
	bool operator<(SharedPtr& other);
	bool operator>(SharedPtr& other);
	bool operator<=(SharedPtr& other);
	bool operator>=(SharedPtr& other);
	bool operator!=(SharedPtr& other);

	bool operator==(std::nullptr_t);
	bool operator<(std::nullptr_t);
	bool operator>(std::nullptr_t);
	bool operator<=(std::nullptr_t);
	bool operator>=(std::nullptr_t);
	bool operator!=(std::nullptr_t);
};

template<class T>
void swap(SharedPtr<T>& ptr1, SharedPtr<T>& ptr2)
{
	 std::swap(ptr1, ptr2);
}

template<class T, class... Args>
SharedPtr<T> MakeShared(Args&&... args);