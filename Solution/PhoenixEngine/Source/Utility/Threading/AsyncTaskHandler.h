#pragma once

#include "Utility/Containers/Queue.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Misc/Function.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/String.h"
#include "Utility/Threading/Atomic.h"
#include "Utility/Threading/ConditionVariable.h"
#include "Utility/Threading/Mutex.h"
#include "Utility/Threading/Thread.h"

namespace Phoenix
{
	typedef TFunction<void()> FAsyncTask;
	typedef TVector<FAsyncTask> FAsyncTasks;

	class FAsyncTaskHandler
	{
	public:
		struct FInitParams
		{
			SizeT MaxThreadCountHint{ 1 };
		};

		FAsyncTaskHandler() = default;

		FAsyncTaskHandler(const FAsyncTaskHandler&) = delete;
		FAsyncTaskHandler& operator=(const FAsyncTaskHandler&) = delete;

		FAsyncTaskHandler(FAsyncTaskHandler&&) = delete;
		FAsyncTaskHandler& operator=(FAsyncTaskHandler&&) = delete;

		~FAsyncTaskHandler();

		void Init(const FInitParams& InitParams);

		bool IsValid() const;

		void Run();

		void ForceShutDown();

		bool IsShutDown() const;

		void AddAsyncTasks(FAsyncTasks&& Tasks);
		
		SizeT GetThreadCount() const;

	private:
		TAtomic<bool> IsRunning{ false };
		TVector<FSafeThread> Threads;
		TAtomic<bool>* IsShutDownVars{ nullptr };

		FMutex Mutex;
		FConditionVariable ConditionVariable;

		FAsyncTasks Tasks;

		void ThreadRunFunc(const SizeT ThreadID);
	};
}
