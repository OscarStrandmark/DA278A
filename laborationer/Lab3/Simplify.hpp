#pragma once

//SimplifyType

template<class T>
struct SimplifyType
{
	using type = T;
};

template<class T>
struct SimplifyType<T*>
{
	using type = T;
};

template<class T>
struct SimplifyType<T&>
{
	using type = T;
};

template<class T>
struct SimplifyType<T[]>
{
	using type = T;
};

template<class T>
struct SimplifyType<const T>
{
	using type = T;
};

template<class T>
using SimplifyType_t = typename SimplifyType<T>::type;

//BaseType

template<class T>
struct BaseType
{
	using type = T;
};

template<class T>
using BaseType_t = typename BaseType<T>::type;

template<class T>
struct BaseType<T*>
{
	using type = typename BaseType_t<T>;
};

template<class T>
struct BaseType<T&>
{
	using type = typename BaseType_t<T>;
};

template<class T>
struct BaseType<T[]>
{
	using type = typename BaseType_t<T>;
};

template<class T>
struct BaseType<const T>
{
	using type = typename BaseType_t<T>;
};
