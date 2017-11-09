#pragma once

#include "Utility/Containers/Queue.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Misc/Function.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Threading/Mutex.h"

namespace Phoenix
{
	class FGFXTaskReceiver
	{
	public:
		typedef TFunction<bool()> FTask;
		typedef TVector<FTask> FTasks;

		FGFXTaskReceiver() = default;

		FGFXTaskReceiver(const FGFXTaskReceiver&) = delete;
		FGFXTaskReceiver& operator=(const FGFXTaskReceiver&) = delete;

		FGFXTaskReceiver(FGFXTaskReceiver&&) = delete;
		FGFXTaskReceiver& operator=(FGFXTaskReceiver&&) = delete;

		~FGFXTaskReceiver();

		void Init();

		void DeInit();

		SizeT GetTasksQueueSize();

		void ReceiveTasks(FTasks&& Tasks);

		void RetrieveTasks(FTasks& Tasks);
		
	private:
		typedef TQueue<FTasks> FTasksQueue;

		FMutex Mutex;
		TQueue<FTasks> TasksQueue;
	};
}
