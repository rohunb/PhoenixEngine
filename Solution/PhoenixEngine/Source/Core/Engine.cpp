#include "Stdafx.h"
#include "Core/Engine.h"

#include "Utility/Debug/Debug.h"
#include "Utility/Debug/Profiler.h"
#include "Utility/FileIO/Endian.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/Timer.h"
#include "Utility/Threading/Thread.h"
#include "Math/Math.h"
#include "Platform/Window/GenericWindow.h"
#include "Components/Cores/RenderCore.h"

using namespace Phoenix;

void FEngine::Run(FGameThread::FCreateGameSceneFunc CreateGameSceneFunc)
{
	Init(CreateGameSceneFunc);

	F_Assert(IsValid(), "Engine failed to initialize.");

	const Float32 FramesPerSec = 120.f;
	const Float32 MaxDeltaTime = 1.f / FramesPerSec;

	TThreadSafeVector<FEvent>::ContainerT ReceivedEvents;
	Float32 AccumulatedTime = 0.f;

	FHighResolutionTimer Timer;
	Timer.Reset();

	while (IsRunning)
	{
		Timer.Update();
		const Float32 DeltaTimeSec = Timer.GetDeltaSeconds<Float32>();
		AccumulatedTime += DeltaTimeSec;

		if (AccumulatedTime >= MaxDeltaTime)
		{
			AccumulatedTime = FMathf::Modulo(AccumulatedTime, MaxDeltaTime);
			Window->ProcessEvents();
			GamePadUtility.ProcessEvents(EventHandler);

			OutgoingEvents.AddData(EventHandler.GetEvents());
			EventHandler.GetEvents().clear();
			IncomingEvents.GetDataAndClear(ReceivedEvents);

			for (SizeT I = 0; I < ReceivedEvents.size(); ++I)
			{
				const FEvent& Event = ReceivedEvents[I];
				HandleEvent(Event);
			}

			ReceivedEvents.clear();
		}

		NThread::SleepThread(1);
	}

	DeInit();
}

void FEngine::Init(FGameThread::FCreateGameSceneFunc CreateGameSceneFunc)
{
	F_LogOpen("Log.txt");
	F_Log("Main Thread ID: " << NThread::GetCallingThreadID());
	F_Assert(CreateGameSceneFunc, "CreateGameSceneFunc is null.");

	FEndian::Init();
	FHighResolutionTimer::GetTimeInSeconds();
	EngineConfig.Read();

	{
		const FWindowConfig& WindowConfig = EngineConfig.GetWindowConfig();

		const FVector2D& WindowDimensions = WindowConfig.GetDimensions();
		const FString& WindowTitle = WindowConfig.GetTitle();

		Window = std::make_shared<FGenericWindow>(WindowDimensions.x, WindowDimensions.y, WindowTitle);
		Window->SetEventHandler(&EventHandler);
	}

	{
		FGameThread::FInitParams InitParams;

		InitParams.Window = Window;
		InitParams.OutgoingEvents = &IncomingEvents;
		InitParams.IncomingEvents = &OutgoingEvents;
		InitParams.CreateGameSceneFunc = CreateGameSceneFunc;

		GameThread.Init(InitParams);
		F_Assert(GameThread.IsValid(), "Game Thread failed to initialize.");
	}

	GamePadUtility.Init(EventHandler);
	F_Assert(GamePadUtility.IsValid(), "GamePadUtility failed to initialize.");

	IsRunning = true;
	F_Assert(IsValid(), "Initialization was successful but this class is not valid.");
}

bool FEngine::IsValid() const
{
	return IsRunning;
}

void FEngine::DeInit()
{
	GamePadUtility.DeInit(EventHandler);
	GameThread.ForceShutDown();
	IsRunning = false;
	F_LogClose();
}

void FEngine::HandleEvent(const FEvent& Event)
{
	switch (Event.Info.Type)
	{
		case EEventType::Engine:
		{
			if (Event.Info.SubType == EEngineEventType::IsShutDown)
			{
				IsRunning = false;
			}
			break;
		}
	}
}
