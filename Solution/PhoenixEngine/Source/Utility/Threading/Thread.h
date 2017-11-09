#ifndef PHOENIX_THREAD_H
#define PHOENIX_THREAD_H

#include <chrono>
#include <thread>

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	typedef std::thread FThread;
	typedef std::thread::id FThreadID;

	namespace NThread
	{
		//	--------------------------------------------------------------------------------
		/*! \brief Puts the thread to sleep for an approximate duration. */
		static inline void SleepThread(const UInt32 Milliseconds)
		{
			const std::chrono::milliseconds Duration(Milliseconds);
			std::this_thread::sleep_for(Duration);
		}

		//	--------------------------------------------------------------------------------
		/*! \brief Returns the thread id of the thread that this function runs on. */
		static inline FThreadID GetCallingThreadID()
		{
			const FThreadID ThreadID = std::this_thread::get_id();
			return ThreadID;
		}

		//	--------------------------------------------------------------------------------
		/*! \brief Returns the number of threads supported by the hardware. */
		static inline SizeT GetHardwareThreadCount()
		{
			const SizeT HardwareThreadCount = std::thread::hardware_concurrency();
			return HardwareThreadCount;
		}
	}

	class FSafeThread
	{
	public:
		FSafeThread() = default;
		//Intentionally non explicit - so it can be constructed directly from an FThread
		FSafeThread(FThread&& Thread);

		FSafeThread(const FSafeThread&) = delete;
		FSafeThread& operator=(const FSafeThread&) = delete;

		FSafeThread(FSafeThread&&) = default;
		FSafeThread& operator=(FSafeThread&&) = default;

		~FSafeThread();

		//Can be removed if we wrap *everything* in std::thread
		FThread& Get();

		void Join();

	private:
		FThread Thread;
	};
}

#endif
