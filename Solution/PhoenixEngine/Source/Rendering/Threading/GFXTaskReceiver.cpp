#include "Stdafx.h"
#include "Rendering/Threading/GFXTaskReceiver.h"

#include "Utility/Misc/Allocator.h"
#include "Math/Math.h"
#include "Rendering/Debug/GFXDebug.h"

using namespace Phoenix;

FGFXTaskReceiver::~FGFXTaskReceiver()
{
	DeInit();
}

void FGFXTaskReceiver::Init()
{
	DeInit();

	// Do nothing for now.
}

void FGFXTaskReceiver::DeInit()
{
	TUniqueLock<FMutex> Lock(Mutex);
	TasksQueue = FTasksQueue();
}

SizeT FGFXTaskReceiver::GetTasksQueueSize()
{
	TUniqueLock<FMutex> Lock(Mutex);
	const SizeT Size = TasksQueue.size();
	return Size;
}

void FGFXTaskReceiver::ReceiveTasks(FTasks&& Tasks)
{
	TUniqueLock<FMutex> Lock(Mutex);
	TasksQueue.push(std::move(Tasks));
}

void FGFXTaskReceiver::RetrieveTasks(FTasks& Tasks)
{
	TUniqueLock<FMutex> Lock(Mutex);
	if (TasksQueue.empty())
	{
		return;
	}

	F_Assert(Tasks.empty(), "Tasks should already be empty.");
	FTasks& NextTaskSet = TasksQueue.front();
	Tasks = std::move(NextTaskSet);
	TasksQueue.pop();
}
