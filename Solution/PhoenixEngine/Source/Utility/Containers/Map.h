#ifndef PHOENIX_MAP_H
#define PHOENIX_MAP_H

#include <map>

namespace Phoenix
{
	template <class Key, class T>
	using TMap = std::map<Key, T>;
}

#endif
