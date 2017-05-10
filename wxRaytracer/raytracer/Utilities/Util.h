#pragma once

#include <algorithm>

template<typename T>
inline bool is_scalar_valid(T val)
{
	if (std::isnan(val) ||
		std::isinf(val))
	{
		printf("The val is INF or NAN!\n");
		return false;
	}

	return true;
}
