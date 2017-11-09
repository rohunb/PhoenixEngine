#include "Stdafx.h"
#include "Core/GameThread.h"

#include "Utility/Containers/BitVector.h"
#include "Utility/Debug/Assert.h"
#include "Utility/Debug/Debug.h"
#include "Utility/Debug/Profiler.h"
#include "Utility/Misc/Algorithm.h"
#include "Utility/Misc/Allocator.h"
#include "Utility/Misc/Timer.h"
#include "ECS/ComponentManagerImpl.h"
#include "Math/Math.h"
#include "Platform/Input/Keys.h"
#include "Platform/Window/IWindow.h"
#include "Rendering/GFXRenderData.h"
#include "Rendering/GFXScene.h"

using namespace Phoenix;

FGameThread::FGameThread() = default; //to allow fwd declare for unique_ptr

bool FGameThread::FInitParams::IsValid() const
{
	const bool Result =
		Window != nullptr &&
		OutgoingEvents != nullptr &&
		IncomingEvents != nullptr &&
		CreateGameSceneFunc != nullptr;

	return Result;
}

FGameThread::~FGameThread()
{
	F_Assert(!IsRunning, "Game Thread should have already been deinitialized.");
	F_Assert(Thread.Get().joinable(), "Game Thread should not have been detached.");

	IsRunning = false;
	Thread.Join();
}

void FGameThread::Init(const FInitParams& InitParams)
{
	F_Assert(InitParams.IsValid(), "Initialization parameters must be valid.");
	F_Assert(!IsRunning, "Game Thread is already running.");

	InitData = InitParams;
	IsRunning = true;
	Thread = FThread(&FGameThread::ThreadRun, this);
}

bool FGameThread::IsValid() const
{
	const bool LocalIsRunning = IsRunning.load();
	return LocalIsRunning;
}

void FGameThread::ForceShutDown()
{
	IsRunning = false;
}

void FGameThread::ThreadRun()
{
	ThreadInit();
	F_LogTrace(F_GetProfiler());
	F_ResetProfiler();

	const Float32 FramesPerSec = 60.f;
	const Float32 MaxDeltaTime = 1.f / FramesPerSec;
	const UInt32 MaxUpdateCountPerFrame = 4;

	TThreadSafeVector<FEvent>::ContainerT ReceivedEvents;
	FUpdateEvent UpdateEvent(0.f);

	Float32 NextFrameProgress = 0.f;
	Float32 AccumulatedTime = 0.f;
	UInt32 UpdateCount = 0;

	FHighResolutionTimer Timer;
	Timer.Reset();

	while (IsRunning)
	{
		{
			F_Profile();

			Timer.Update();
			const Float32 DeltaTimeSec = Timer.GetDeltaSeconds<Float32>();

			AccumulatedTime += DeltaTimeSec;
			UpdateCount = 0;

			NextFrameProgress = AccumulatedTime / MaxDeltaTime;
			while (AccumulatedTime >= MaxDeltaTime)
			{
				F_ProfileWithMsg("Update");
				UpdateEvent.CurrentTimeS = Timer.GetTimeInSeconds();
				AccumulatedTime -= MaxDeltaTime;

				const bool bIsFirstUpdateThisFrame = UpdateCount == 0;
				if (bIsFirstUpdateThisFrame)
				{
					const Float32 AccumTModDeltaT = FMathf::Modulo(AccumulatedTime, MaxDeltaTime);

					UpdateEvent.PreviousFrameTimeS = UpdateEvent.CurrentFrameTimeS;
					UpdateEvent.CurrentFrameTimeS = UpdateEvent.CurrentTimeS - AccumTModDeltaT;
					UpdateEvent.DeltaTimeS = MaxDeltaTime;

					NextFrameProgress = AccumTModDeltaT / MaxDeltaTime;
				}

				InitData.IncomingEvents->GetDataAndClear<false>(ReceivedEvents);
				ThreadHandleEvents(UpdateEvent, ReceivedEvents);

				GameScene->Update(UpdateEvent, ComponentManagerImpl->ComponentManager);

				ComponentManagerImpl->ComponentManager.UpdateSystems(UpdateEvent);
				ComponentManagerImpl->ComponentManager.Refresh();

				++UpdateCount;
				const UInt32 MinFramesBeforeWarning = 2;
				const bool TwoUpdatesForFrame = UpdateCount == MinFramesBeforeWarning;
				F_LogWarningIf(TwoUpdatesForFrame, "Two or more updates occurred.  AccT: " << AccumulatedTime);

				if (UpdateCount >= MaxUpdateCountPerFrame)
				{
					AccumulatedTime = FMathf::Modulo(AccumulatedTime, MaxDeltaTime);
					NextFrameProgress = AccumulatedTime / MaxDeltaTime;
					F_LogError("Hit the maximum update count of " << MaxUpdateCountPerFrame 
						<< " for this frame.  Reducing AccumulatedTime to " << AccumulatedTime);
				}
			}

			if (UpdateCount)
			{
				GFXEngine.SetUpNextRender();
			}

			// Eliminate spin wait until we actually need the CPU usage.
			if (UpdateCount == 0)
			{
				NThread::SleepThread(1);
			}
		}

		F_ResetProfiler();
	}

	ThreadDeInit();
}

void FGameThread::ThreadInit()
{
	F_Profile();
	F_Log("Game Thread ID: " << NThread::GetCallingThreadID());

	{
		AudioEngine.Init();
		F_Assert(AudioEngine.IsValid(), "Audio Engine failed to initialize.");
	}

	{
		// #FIXME: Init physics here.
	}

	{
		F_Assert(InitData.Window, "Window is null.");
		FGFXEngine::FInitParams InitParams;
		InitParams.Window = InitData.Window.get();

		GFXEngine.Init(InitParams);
		F_Assert(GFXEngine.IsValid(), "GFXEngine failed to initialize.");
	}

	{
		F_ProfileWithMsg("ComponentManager Instantiate");
		ComponentManagerImpl = std::make_unique<FComponentManagerImpl>();
	}
	
	{
		GameScene = InitData.CreateGameSceneFunc();
		F_Assert(GameScene, "GameScene is null.");
		GameScene->Init(ComponentManagerImpl->ComponentManager);
	}

	{
		F_ProfileWithMsg("ComponentManager Render System Init");
		auto& ComponentManager = ComponentManagerImpl->ComponentManager;
		auto& RenderSystem = ComponentManager.GetSystem<FComponentRegistry::RenderSystem>();
		RenderSystem.Init(ComponentManager, GFXEngine.GetScene());
	}
	{
		F_ProfileWithMsg("ComponentManager Systems Init");
		//Init all systems
		ComponentManagerImpl->ComponentManager.InitSystems();
	}

	F_LogTrace("GameThread::ThreadInit()\n");
}

void FGameThread::ThreadDeInit()
{
	{
		F_ResetProfiler();
		F_Profile();
		F_LogTrace("GameThread::ThreadDeInit()");

		if (GameScene)
		{
			GameScene->DeInit();
			GameScene = nullptr;
		}

		ComponentManagerImpl->ComponentManager.DeInitSystems();
		GFXEngine.ForceShutDown();
		// #FIXME: DeInit Physics
		AudioEngine.DeInit();

		FEvent Event;
		Event.Info = FEventInfo(EEventType::Engine, EEngineEventType::IsShutDown);

		InitData.OutgoingEvents->AddEntry(Event);
		InitData = FInitParams();
	}

	// Wait for other systems to be shut down.
	while (!GFXEngine.IsShutDown())
	{
		NThread::SleepThread(1);
	}

	F_LogTrace(F_GetProfiler());
	F_ResetProfiler();
}

void FGameThread::ThreadHandleEvents(const FUpdateEvent& UpdateEvent, TThreadSafeVector<FEvent>::ContainerT& ReceivedEvents)
{
#if _DEBUG
	for (SizeT I = 1, Max = ReceivedEvents.size(); I < Max; ++I)
	{
		const FEvent& Event1 = ReceivedEvents[I - 1];
		const FEvent& Event2 = ReceivedEvents[I];
		F_Assert(Event1.Info.TimeStamp <= Event2.Info.TimeStamp, "A more recent event's time stamp is invalid.");
	}
#endif

#if 0 // #FIXME: Not working fully yet.
	const SizeT Max = ReceivedEvents.size();
	SizeT EventsHandled = 0;
	
	for (SizeT I = 0; I < Max; ++I)
	{
		const FEvent& Event = ReceivedEvents[I];
		if (Event.Info.TimeStamp <= UpdateEvent.CurrentFrameTimeS)
		{
			++EventsHandled;
			ThreadHandleEvent(Event);
		}

		break;
	}

	ReceivedEvents.erase(ReceivedEvents.begin(), ReceivedEvents.begin() + EventsHandled);
#else

	// #FIXME: Only handle events up to the current time stamp.
	for (SizeT I = 0; I < ReceivedEvents.size(); ++I)
	{
		const FEvent& Event = ReceivedEvents[I];
		ThreadHandleEvent(Event);
	}

	ReceivedEvents.clear();
#endif
}

void FGameThread::ThreadHandleEvent(const FEvent& Event)
{
	switch (Event.Info.Type)
	{
		case EEventType::Engine:
		{
			if (Event.Info.SubType == EEngineEventType::ShutDownNow)
			{
				IsRunning = false;
			}
			break;
		}

		case EEventType::Key:
		{
			const FKeyEvent& KeyEvent = Event.Key;

			F_Assert(KeyEvent.Info.SubType == EKeyEventType::Default, "This function expects the default type.");
			if (KeyEvent.Action == EInputAction::Press)
			{
				switch (KeyEvent.Key)
				{
					case EKey::Escape:
					{
						IsRunning = false;
						break;
					}
				}
			}

			break;
		}

		case EEventType::Window:
		{
			const FWindowEvent& WindowEvent = Event.Window;

			if (WindowEvent.Info.SubType == EWindowEventType::CloseAttempt)
			{
				IsRunning = false;
			}
		}
	}

	ComponentManagerImpl->ComponentManager.NotifySystemsOnEvent(Event);

	static_assert(EEventType::Count == 9, "This table requires updating.");
	static const FBitVector LoggedEventsLookUpTable =
	{
		/* Unknown	*/false,
		/* Engine	*/false,
		/* GamePad	*/false,
		/* Key		*/false,
		/* MoveAxis	*/false,
		/* Mouse	*/false,
		/* Update	*/false,
		/* User		*/false,
		/* Window	*/false
	};

	F_LogTraceIf(LoggedEventsLookUpTable[Event.Info.Type], Event);

	// #FIXME: Pass it to the main event handler, which 
	// will then fire off the events as required.
}
