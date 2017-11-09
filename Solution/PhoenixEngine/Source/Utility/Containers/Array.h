#ifndef PHOENIX_ARRAY_H
#define PHOENIX_ARRAY_H

#include <array>

namespace Phoenix
{
	template <class T, size_t Size>
	using TArray = std::array<T, Size>;
}

#endif
