#ifndef PHOENIX_PRIORITY_QUEUE_H
#define PHOENIX_PRIORITY_QUEUE_H

#include <queue>

namespace Phoenix
{
	template <class T, class Container, class Compare>
	using TPriorityQueue = std::priority_queue<T, Container, Compare>;
}

#endif
