#ifndef PHOENIX_GAME_THREAD_H
#define PHOENIX_GAME_THREAD_H

#include "Utility/Handle/Handle.h"
#include "Utility/Misc/Function.h"
#include "Utility/Misc/Memory.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Threading/Atomic.h"
#include "Utility/Threading/Thread.h"
#include "Utility/Threading/ThreadSafeVector.h"
#include "Platform/Event/Event.h"
#include "Audio/AudioEngine.h"
#include "Rendering/GFXEngine.h"
#include "Rendering/Image/ImageInstance.h"
#include "Rendering/Model/ModelInstance.h"
#include "Rendering/Text/TextInstance.h"
#include "Core/GameScene.h"

namespace Phoenix
{
	class FGameThread
	{
	public:
		typedef TUniquePtr<FGameScene>(*FCreateGameSceneFunc)();

		struct FInitParams
		{
			TSharedPtr<class IWindow> Window;
			TThreadSafeVector<FEvent>* OutgoingEvents{ nullptr };
			TThreadSafeVector<FEvent>* IncomingEvents{ nullptr };
			FCreateGameSceneFunc CreateGameSceneFunc{ nullptr };

			bool IsValid() const;
		};

		FGameThread();

		FGameThread(const FGameThread&) = delete;
		FGameThread& operator=(const FGameThread&) = delete;

		FGameThread(FGameThread&&) = delete;
		FGameThread& operator=(FGameThread&&) = delete;

		~FGameThread();

		void Init(const FInitParams& InitParams);

		bool IsValid() const;

		void ForceShutDown();

	protected:
	private:
		FSafeThread Thread;
		FInitParams InitData;
		FAudioEngine AudioEngine;
		FGFXEngine GFXEngine;
		TUniquePtr<FGameScene> GameScene;
		TUniquePtr<struct FComponentManagerImpl> ComponentManagerImpl;

		TAtomic<bool> IsRunning{ false };

		void ThreadRun();

		void ThreadInit();

		void ThreadDeInit();

		void ThreadHandleEvents(const FUpdateEvent& UpdateEvent, TThreadSafeVector<FEvent>::ContainerT& ReceivedEvents);

		void ThreadHandleEvent(const FEvent& Event);
	};
}

#endif
