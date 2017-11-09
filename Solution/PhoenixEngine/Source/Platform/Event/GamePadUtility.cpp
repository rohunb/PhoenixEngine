#include "Stdafx.h"
#include "Platform/Event/GamePadUtility.h"

#include "ExternalLib/GLFWIncludes.h"
#include "Utility/Misc/Timer.h"
#include "Utility/Misc/Memory.h"
#include "Math/Math.h"
#include "Platform/Event/EventHandler.h"

using namespace Phoenix;

const Float32 FGamePadState::AxisPickUpRange = 0.05f;

FGamePadState::~FGamePadState()
{
	F_Assert(!IsValid(), "GamePadState should have already been deinitialized.");
}

void FGamePadState::Init(const EGamePadID::Value InGamePadID, FEventHandler& EventHandler)
{
	DeInit(EventHandler);

	GamePadID = InGamePadID;

	const Float32 TimeStamp = FHighResolutionTimer::GetTimeInSeconds();
	EventHandler.GamePadEventCallback(EGamePadEventType::Connected, GamePadID, TimeStamp);

	InitAxes(EventHandler);
	InitButtons(EventHandler);

	F_Assert(IsValid(), "Initialization succeeded but this class is invalid.");
}

bool FGamePadState::IsValid() const
{
	const bool Result = GamePadID != EGamePadID::Unknown;
	return Result;
}

void FGamePadState::DeInit(FEventHandler& EventHandler)
{
	if (GamePadID != EGamePadID::Unknown)
	{
		const Float32 TimeStamp = FHighResolutionTimer::GetTimeInSeconds();

		ResetAxes(EventHandler);
		ResetButtons(EventHandler);
		EventHandler.GamePadEventCallback(EGamePadEventType::Disconnected, GamePadID, TimeStamp);
	}

	GamePadID = EGamePadID::Unknown;
	Axes.clear();
	Buttons.clear();
}

void FGamePadState::ProcessEvents(FEventHandler& EventHandler)
{
	ProcessAxisEvents(EventHandler);
	ProcessButtonEvents(EventHandler);
}

const FChar* FGamePadState::GetName() const
{
	F_Assert(IsValid(), "This class must be valid.");
	const FChar* Name = glfwGetJoystickName(GamePadID);
	F_Assert(Name, "Failed to retrieve JoystickName for GamePadID: " << GamePadID);

	return Name;
}

void FGamePadState::InitAxes(FEventHandler& EventHandler)
{
	F_Assert(IsValid(), "This class must be valid.");

	Int32 AxisCount = 0;
	TRawPtr<const Float32> AxesArray = glfwGetJoystickAxes(GamePadID, &AxisCount);
	F_Assert(AxesArray.IsValid(), "Failed to retrieve JoystickAxes for GamePadID: " << GamePadID);

	Axes.resize(AxisCount);
	std::copy(AxesArray.Get(), AxesArray.Get() + AxisCount, Axes.begin());

	for (SizeT I = 0; I < Axes.size(); ++I)
	{
		const auto& Axis = Axes[I];
		if (!FMathf::IsCloseTo(Axis, 0.f, AxisPickUpRange))
		{
			const EGamePadAxis::Value Axis = static_cast<EGamePadAxis::Value>(I);
			const Float32 TimeStamp = FHighResolutionTimer::GetTimeInSeconds();

			EventHandler.GamePadEventCallback(GamePadID, Axis, 0.f, Axis, TimeStamp);
		}
	}
}

void FGamePadState::InitButtons(FEventHandler& EventHandler)
{
	F_Assert(IsValid(), "This class must be valid.");

	Int32 ButtonCount = 0;
	TRawPtr<const UInt8> ButtonsArray = glfwGetJoystickButtons(GamePadID, &ButtonCount);
	F_Assert(ButtonsArray.IsValid(), "Failed to retrieve JoystickButtons for GamePadID: " << GamePadID);

	Buttons.resize(ButtonCount);
	std::copy(ButtonsArray.Get(), ButtonsArray.Get() + ButtonCount, Buttons.begin());

	for (SizeT I = 0; I < Buttons.size(); ++I)
	{
		const auto& Button = Buttons[I];
		if (Button == EInputAction::Press)
		{
			const EGamePadButton::Value GPButton = static_cast<EGamePadButton::Value>(I);
			const Float32 TimeStamp = FHighResolutionTimer::GetTimeInSeconds();

			EventHandler.GamePadEventCallback(GamePadID, GPButton, EInputAction::Press, TimeStamp);
		}
	}
}

void FGamePadState::ResetAxes(FEventHandler& EventHandler)
{
	for (SizeT I = 0; I < Axes.size(); ++I)
	{
		auto& Axis = Axes[I];
		if (Axis != 0.f)
		{
			const EGamePadAxis::Value GPAxis = static_cast<EGamePadAxis::Value>(I);
			const Float32 TimeStamp = FHighResolutionTimer::GetTimeInSeconds();

			EventHandler.GamePadEventCallback(GamePadID, GPAxis, Axis, 0.f, TimeStamp);
			Axis = 0.f;
		}
	}
}

void FGamePadState::ResetButtons(FEventHandler& EventHandler)
{
	for (SizeT I = 0; I < Buttons.size(); ++I)
	{
		auto& Button = Buttons[I];
		if (Button == EInputAction::Press)
		{
			const EGamePadButton::Value GPButton = static_cast<EGamePadButton::Value>(I);
			const Float32 TimeStamp = FHighResolutionTimer::GetTimeInSeconds();

			EventHandler.GamePadEventCallback(GamePadID, GPButton, EInputAction::Release, TimeStamp);
			Button = EInputAction::Release;
		}
	}
}

void FGamePadState::ProcessAxisEvents(FEventHandler& EventHandler)
{
	Int32 AxisCount = 0;
	TRawPtr<const Float32> AxesArray = glfwGetJoystickAxes(GamePadID, &AxisCount);

	if (!AxesArray.IsValid())
	{
		F_Assert(false, "Failed to retrieve JoystickAxes for GamePadID: " << GamePadID);
		ResetAxes(EventHandler);
		return;
	}

	const SizeT AxesArraySize = Axes.size();

	const bool ArraySizesMatch = AxesArraySize == static_cast<SizeT>(AxisCount);
	if (!ArraySizesMatch)
	{
		F_Assert(false, "Array size mismatch.  Size: " << AxesArraySize << ", Expected: " << AxisCount);
		ResetAxes(EventHandler);
		InitAxes(EventHandler);
		return;
	}

	for (SizeT I = 0; I < AxesArraySize; ++I)
	{
		if (!FMathf::IsCloseTo(Axes[I], AxesArray[I], AxisPickUpRange))
		{
			const EGamePadAxis::Value GPAxis = static_cast<EGamePadAxis::Value>(I);
			const Float32 PrevAxis = Axes[I];
			const Float32 CurrAxis = AxesArray[I];
			const Float32 TimeStamp = FHighResolutionTimer::GetTimeInSeconds();

			EventHandler.GamePadEventCallback(GamePadID, GPAxis, PrevAxis, CurrAxis, TimeStamp);
			Axes[I] = AxesArray[I];
		}
	}
}

void FGamePadState::ProcessButtonEvents(FEventHandler& EventHandler)
{
	Int32 ButtonCount = 0;
	TRawPtr<const UInt8> ButtonsArray = glfwGetJoystickButtons(GamePadID, &ButtonCount);

	if (!ButtonsArray.IsValid())
	{
		F_Assert(false, "Failed to retrieve ButtonsArray for GamePadID: " << GamePadID);
		ResetButtons(EventHandler);
		return;
	}

	const SizeT ButtonsArraySize = Buttons.size();
	const bool ArraySizesMatch = ButtonsArraySize == static_cast<SizeT>(ButtonCount);
	if (!ArraySizesMatch)
	{
		F_Assert(false, "Array size mismatch.  Size: " << ButtonsArraySize << ", Expected: " << ButtonCount);
		ResetButtons(EventHandler);
		InitButtons(EventHandler);
		return;
	}

	for (SizeT I = 0; I < ButtonsArraySize; ++I)
	{
		if (ButtonsArray[I] != Buttons[I])
		{
			const EGamePadButton::Value GPButton = static_cast<EGamePadButton::Value>(I);
			const EInputAction::Value InputAction = ButtonsArray[I] == EInputAction::Press ?
				EInputAction::Press : EInputAction::Release;

			const Float32 TimeStamp = FHighResolutionTimer::GetTimeInSeconds();

			EventHandler.GamePadEventCallback(GamePadID, GPButton, InputAction, TimeStamp);
			Buttons[I] = ButtonsArray[I];
		}
	}
}

const EGamePadID::Type FGamePadUtility::MaxGamePadsConfig = 4;

FGamePadUtility::FGamePadUtility()
{
	ActiveGamePads.reset();
}

FGamePadUtility::~FGamePadUtility()
{
	F_Assert(ActiveGamePads.count() == 0, "GamePadUtility should have already been deinitialized.");
}

void FGamePadUtility::Init(FEventHandler& EventHandler)
{
	DeInit(EventHandler);
	ProcessEvents(EventHandler);
}

bool FGamePadUtility::IsValid() const
{
	return true;
}

void FGamePadUtility::DeInit(FEventHandler& EventHandler)
{
	for (SizeT I = 0; I < GamePads.size(); ++I)
	{
		if (ActiveGamePads[I])
		{
			GamePads[I].DeInit(EventHandler);
		}
	}

	ActiveGamePads.reset();
}

void FGamePadUtility::ProcessEvents(FEventHandler& EventHandler)
{
	for (EGamePadID::Type I = 0; I < MaxGamePadsConfig; ++I)
	{
		const Int32 IsPresent = glfwJoystickPresent(I);
		const bool IsAvailable = IsPresent == GL_TRUE;

		const EGamePadID::Value GamePadID = static_cast<EGamePadID::Value>(I);
		const bool GPIsActive = GamePadIsActive(GamePadID);

		if (!IsAvailable)
		{
			if (GPIsActive)
			{
				GamePads[I].DeInit(EventHandler);
				SetGamePadState(GamePadID, false);
			}

			continue;
		}

		if (!GPIsActive)
		{
			GamePads[I].Init(GamePadID, EventHandler);
			SetGamePadState(GamePadID, true);
			continue;
		}

		GamePads[I].ProcessEvents(EventHandler);
	}
}

void FGamePadUtility::SetGamePadState(const EGamePadID::Value GamePadID, const bool IsActive)
{
	ActiveGamePads[GamePadID] = IsActive;
}

bool FGamePadUtility::GamePadIsActive(const EGamePadID::Value GamePadID) const
{
	const bool Result = ActiveGamePads[GamePadID];
	return Result;
}
