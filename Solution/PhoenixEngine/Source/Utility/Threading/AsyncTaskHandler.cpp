#include "Stdafx.h"
#include "Utility/Threading/AsyncTaskHandler.h"

#include "Utility/Misc/Allocator.h"
#include "Utility/Debug/Debug.h"
#include "Math/Math.h"

using namespace Phoenix;

FAsyncTaskHandler::~FAsyncTaskHandler()
{
	for (auto& Thread : Threads)
	{
		Thread.Join();
	}

	FAlloc::DeleteArray(IsShutDownVars);
}

void FAsyncTaskHandler::Init(const FInitParams& InitParams)
{
	F_Assert(!IsRunning.load(), "Task handler should not already be running.");
	
	const SizeT HardwareThreadCount = NThread::GetHardwareThreadCount();

	const SizeT MinThreads = 1;
	const SizeT MaxThreads = HardwareThreadCount != 0 ? HardwareThreadCount : 1;

	// #FIXME: Remove the Math layer dependency (the clamp function should probably be in algorithm.h).
	const SizeT MaxThreadCount = TMath<SizeT>::Clamp(InitParams.MaxThreadCountHint, MinThreads, MaxThreads);

	Threads.resize(MaxThreadCount);

	FAlloc::DeleteArray(IsShutDownVars);
	IsShutDownVars = FAlloc::NewArray<TAtomic<bool>>(Threads.size());

	for (SizeT I = 0, Max = Threads.size(); I < Max; ++I)
	{
		IsShutDownVars[I] = true;
	}
}

bool FAsyncTaskHandler::IsValid() const
{
	const bool LocalIsRunning = IsRunning.load();
	return LocalIsRunning;
}

void FAsyncTaskHandler::Run()
{
	F_Assert(!IsRunning.load(), "Task handler should not already be running.");
	F_Assert(IsShutDown(), "Task handler should be shutdown.");
	IsRunning = true;

	const SizeT Max = Threads.size();
	for (SizeT I = 0; I < Max; ++I)
	{
		IsShutDownVars[I] = false;
	}

	for (SizeT I = 0; I < Max; ++I)
	{
		Threads[I] = FThread(&FAsyncTaskHandler::ThreadRunFunc, this, I);
	}
}

void FAsyncTaskHandler::ForceShutDown()
{
	IsRunning = false;

	TUniqueLock<FMutex> Lock(Mutex);
	ConditionVariable.notify_all();
}

bool FAsyncTaskHandler::IsShutDown() const
{
	if (!IsShutDownVars)
	{
		return true;
	}

	for (SizeT I = 0, Max = Threads.size(); I < Max; ++I)
	{
		if (!IsShutDownVars[I].load())
		{
			return false;
		}
	}

	return true;
}

void FAsyncTaskHandler::AddAsyncTasks(FAsyncTasks&& InTasks)
{
	if (InTasks.empty())
	{
		return;
	}

	TUniqueLock<FMutex> Lock(Mutex);

	for (auto& Task : InTasks)
	{
		Tasks.emplace_back(std::move(Task));
	}

	InTasks.clear();
	ConditionVariable.notify_all();
}

SizeT FAsyncTaskHandler::GetThreadCount() const
{
	const SizeT ThreadCount = Threads.size();
	return ThreadCount;
}

void FAsyncTaskHandler::ThreadRunFunc(const SizeT ThreadID)
{
	F_Log("FAsyncTaskHandler Thread #" << ThreadID << ", Thread ID: " << NThread::GetCallingThreadID());

	FAsyncTask AsyncTask;
	while (IsRunning)
	{
		bool DidGetTask = false;

		{
			TUniqueLock<FMutex> Lock(Mutex);

			if (Tasks.empty())
			{
				ConditionVariable.wait(Lock);
			}

			if (!Tasks.empty())
			{
				AsyncTask = std::move(Tasks.back());
				Tasks.pop_back();
				DidGetTask = true;
			}
		}

		if (DidGetTask)
		{
			AsyncTask();
		}
	}

	IsShutDownVars[ThreadID] = true;
}
