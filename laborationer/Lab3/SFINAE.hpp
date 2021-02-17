#pragma once

template<class T, class c = typename std::enable_if_t<std::is_nothrow_copy_constructible_v<T>>>
bool NoThrowCopyConstructible(T t)
{
	return true;
}

template<class T, class C = typename std::enable_if_t<!std::is_nothrow_copy_constructible_v<T>,int>>
bool NoThrowCopyConstructible(T t,C = C())
{
	return false;
}