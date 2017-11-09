#ifndef PHOENIX_BIT_ARRAY_H
#define PHOENIX_BIT_ARRAY_H

#include <bitset>

namespace Phoenix
{
	template <size_t Size>
	using TBitArray = std::bitset<Size>;
}
#endif
