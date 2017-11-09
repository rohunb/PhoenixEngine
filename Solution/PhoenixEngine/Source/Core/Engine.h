#ifndef PHOENIX_ENGINE_H
#define PHOENIX_ENGINE_H

#include "Core/GameThread.h"
#include "Config/EngineConfig.h"
#include "Platform/Event/Event.h"
#include "Platform/Event/EventHandler.h"
#include "Platform/Event/GamePadUtility.h"
#include "Utility/Misc/Memory.h"
#include "Utility/Threading/ThreadSafeVector.h"

namespace Phoenix
{
	class FEngine
	{

	public:

		FEngine() = default;

		FEngine(const FEngine&) = delete;
		FEngine& operator=(const FEngine&) = delete;

		FEngine(FEngine&&) = delete;
		FEngine& operator=(FEngine&&) = delete;
		
		~FEngine() = default;

		void Run(FGameThread::FCreateGameSceneFunc CreateGameSceneFunc);

	private:

		FEngineConfig EngineConfig;

		TSharedPtr<class IWindow> Window;
		FGameThread GameThread;

		FGamePadUtility GamePadUtility;
		FEventHandler EventHandler;
		TThreadSafeVector<FEvent> OutgoingEvents;
		TThreadSafeVector<FEvent> IncomingEvents;

		bool IsRunning{ false };

		void Init(FGameThread::FCreateGameSceneFunc CreateGameSceneFunc);

		bool IsValid() const;

		void DeInit();

		void HandleEvent(const FEvent& Event);

	};
}

#endif
