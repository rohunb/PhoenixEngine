#include "Stdafx.h"
#include "Platform/Event/Event.h"

using namespace Phoenix;

FEventInfo::FEventInfo(
	const EEvent::TypeT InType,
	const EEvent::SubTypeT InSubType)
	: FEventInfo(InType, InSubType, 0.f)
{
}

FEventInfo::FEventInfo(
	const EEvent::TypeT InType,
	const EEvent::SubTypeT InSubType,
	const Float32 InTimeStamp)
	: Type(InType)
	, SubType(InSubType)
	, TimeStamp(InTimeStamp)
{
}

FGamePadEvent::FGamePadEvent(
	const EGamePadEventType::Value SubType,
	const EGamePadID::Value ID,
	const Float32 TimeStamp)
	: FGamePadEvent(
		SubType, 
		ID,
		EGamePadButton::None,
		EInputAction::None,
		EGamePadAxis::None,
		0.f,
		0.f,
		TimeStamp)
{
}

FGamePadEvent::FGamePadEvent(
	const EGamePadID::Value ID,
	const EGamePadButton::Value Button,
	const EInputAction::Value Action,
	const Float32 TimeStamp)
	: FGamePadEvent(
		EGamePadEventType::Default,
		ID,
		Button,
		Action,
		EGamePadAxis::None,
		0.f,
		0.f,
		TimeStamp)
{
}

FGamePadEvent::FGamePadEvent(
	const EGamePadID::Value ID,
	const EGamePadAxis::Value Axis,
	const Float32 PrevAxis,
	const Float32 CurrAxis,
	const Float32 TimeStamp)
	: FGamePadEvent(
		EGamePadEventType::Default,
		ID,
		EGamePadButton::None,
		EInputAction::None,
		Axis,
		PrevAxis,
		CurrAxis,
		TimeStamp)
{
}

FGamePadEvent::FGamePadEvent(
	const EGamePadEventType::Value SubType,
	const EGamePadID::Value InID,
	const EGamePadButton::Value InButton,
	const EInputAction::Value InAction,
	const EGamePadAxis::Value InAxis,
	const Float32 InPrevAxis,
	const Float32 InCurrAxis,
	const Float32 TimeStamp)
	: Info(EEventType::GamePad, SubType, TimeStamp)
	, ID(InID)
	, Button(InButton)
	, Action(InAction)
	, Axis(InAxis)
	, PrevAxis(InPrevAxis)
	, CurrAxis(InCurrAxis)
{
}

FKeyEvent::FKeyEvent(
	const EKey::Value InKey,
	const EInputAction::Value InAction,
	const EKeyMods::BitMask InMods,
	const Float32 TimeStamp)
	: Info(EEventType::Key, EKeyEventType::Default, TimeStamp)
	, Key(InKey)
	, Action(InAction)
	, Mods(InMods)
{
}

FMovementAxisEvent::FMovementAxisEvent(const Float32 HorizontalAxis, const Float32 VerticalAxis, const Float32 TimeStamp)
	: Info(EEventType::MovementAxis, EMovementAxisEventType::Default, TimeStamp)
	, HorizontalAxis(HorizontalAxis)
	, VerticalAxis(VerticalAxis)
{
}

FMouseEvent::FMouseEvent(
	const EMouseEventType::Value SubType,
	const Float32 InX,
	const Float32 InY,
	const Float32 TimeStamp)
	: FMouseEvent(
		SubType,
		EMouseButton::Unknown,
		EInputAction::None,
		EKeyMods::None,
		InX,
		InY,
		EMouseEventInfo::None,
		TimeStamp)
{
}

FMouseEvent::FMouseEvent(
	const EMouseEventInfo::Type MEInfo,
	const Float32 TimeStamp)
	: FMouseEvent(
		EMouseEventType::Misc,
		EMouseButton::Unknown,
		EInputAction::None,
		EKeyMods::None,
		0.f,
		0.f,
		MEInfo,
		TimeStamp)
{
}

FMouseEvent::FMouseEvent(
	const EMouseButton::Value Button,
	const EInputAction::Value Action,
	const EKeyMods::BitMask Mods,
	const Float32 TimeStamp)
	: FMouseEvent(
		EMouseEventType::Click,
		Button,
		Action,
		Mods,
		0.f,
		0.f,
		EMouseEventInfo::None,
		TimeStamp)
{
}

FMouseEvent::FMouseEvent(
	const EMouseEventType::Value SubType,
	const EMouseButton::Value InButton,
	const EInputAction::Value InAction,
	const EKeyMods::BitMask InMods,
	const Float32 InX,
	const Float32 InY,
	const EMouseEventInfo::Type InMEInfo,
	const Float32 TimeStamp)
	: Info(EEventType::Mouse, SubType, TimeStamp)
	, Button(InButton)
	, Action(InAction)
	, Mods(InMods)
	, X(InX)
	, Y(InY)
	, MEInfo(InMEInfo)
{
}

FUpdateEvent::FUpdateEvent(const Float32 TimeStamp)
	: Info(EEventType::Update, EUpdateEventType::Default, TimeStamp)
	, PreviousFrameTimeS(0.f)
	, CurrentFrameTimeS(0.f)
	, CurrentTimeS(0.f)
	, DeltaTimeS(0.f)
{
}

FUserEvent::FUserEvent(
	const EEvent::SubTypeT SubType,
	void* const InPtr1,
	void* const InPtr2,
	const Float32 TimeStamp)
	: Info(EEventType::User, SubType, TimeStamp),
	Ptr1(InPtr1),
	Ptr2(InPtr2)
{
}

const UInt8 FWindowEvent::UnsetValue = 0;

FWindowEvent::FWindowEvent(
	const EWindowEventType::Value SubType,
	const EWindowEventInfo::Type InWEInfo,
	const Float32 TimeStamp)
	: FWindowEvent(SubType, UnsetValue, UnsetValue, InWEInfo, TimeStamp)
{
}

FWindowEvent::FWindowEvent(
	const EWindowEventType::Value SubType,
	const ValueT X,
	const ValueT Y,
	const Float32 TimeStamp)
	: FWindowEvent(SubType, X, Y, EWindowEventInfo::None, TimeStamp)
{
}

FWindowEvent::FWindowEvent(
	const EWindowEventType::Value SubType,
	const ValueT InX,
	const ValueT InY,
	const EWindowEventInfo::Type InWEInfo,
	const Float32 TimeStamp)
	: Info(EEventType::Window, SubType, TimeStamp)
	, X(InX)
	, Y(InY)
	, WEInfo(InWEInfo)
{
}
