#ifndef PHOENIX_SET_H
#define PHOENIX_SET_H

#include <set>

namespace Phoenix
{
	template<class T, class Compare>
	using TSet = std::set<T, Compare>;
}

#endif
