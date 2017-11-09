#ifndef PHOENIX_UNORDERED_MAP_H
#define PHOENIX_UNORDERED_MAP_H

#include <unordered_map>

namespace Phoenix
{
	template <class Key, class Value>
	using TUnorderedMap = std::unordered_map<Key, Value>;
}

#endif
