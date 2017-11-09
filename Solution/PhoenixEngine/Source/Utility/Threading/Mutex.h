#ifndef PHOENIX_MUTEX_H
#define PHOENIX_MUTEX_H

#include <mutex>

namespace Phoenix
{
	typedef std::mutex FMutex;
	typedef std::timed_mutex FTimedMutex;

	template <class T>
	using TLockGuard = std::lock_guard<T>;

	template <class T>
	using TUniqueLock = std::unique_lock<T>;

	typedef TLockGuard<FMutex> FMutexLock;
	typedef TLockGuard<FTimedMutex> FTimedMutexLock;
}

#endif
