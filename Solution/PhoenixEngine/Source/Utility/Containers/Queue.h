#ifndef PHOENIX_QUEUE_H
#define PHOENIX_QUEUE_H

#include <queue>

namespace Phoenix
{
	template <class T>
	using TDeque = std::deque<T>;

	template <class T, class Container = TDeque<T>>
	using TQueue = std::queue<T, Container>;
}

#endif
