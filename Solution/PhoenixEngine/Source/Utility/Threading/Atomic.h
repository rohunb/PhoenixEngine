#ifndef PHOENIX_ATOMIC_H
#define PHOENIX_ATOMIC_H

#include <atomic>

namespace Phoenix
{
	template <class T>
	using TAtomic = std::atomic<T>;
}

#endif
