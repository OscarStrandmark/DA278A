#pragma once
#include <type_traits>

template <class CONT>
auto Sum(const CONT& c)
{
	using unknown_type = decltype(*std::begin(c));
	using unknown_type_scrubbed = typename std::remove_const<std::remove_reference<unknown_type>::type>::type;;
	
	unknown_type_scrubbed sum{};

	for (auto e : c)
	{
		sum += e;
	}

	return sum;
}