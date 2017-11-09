#ifndef PHOENIX_EVENT_H
#define PHOENIX_EVENT_H

#include "Utility/Debug/Assert.h"
#include "Utility/Debug/Debug.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/OutputStream.h"
#include "Platform/Event/EventTypes.h"
#include "Platform/Input/GamePadEnums.h"
#include "Platform/Input/InputAction.h"
#include "Platform/Input/KeyMods.h"
#include "Platform/Input/Keys.h"
#include "Platform/Input/MouseButton.h"

namespace Phoenix
{
	namespace EMouseEventInfo
	{
		typedef UInt8 Type;

		enum BitMask : Type
		{
			None,
			IsDidEnterWindowSet = 1 << 0,
			DidEnterWindow = 1 << 1
		};
	}

	namespace EWindowEventInfo
	{
		typedef UInt8 Type;

		enum BitMask : Type
		{
			None,
			IsFocusedSet = 1 << 0,
			IsFocused = 1 << 1,
			IsIconifiedSet = 1 << 2,
			IsIconified = 1 << 3,
		};
	}

	struct FEventInfo
	{
		friend struct FEvent;
		friend struct FEngineEvent;
		friend struct FGamePadEvent;
		friend struct FKeyEvent;
		friend struct FMovementAxisEvent;
		friend struct FMouseEvent;
		friend struct FUpdateEvent;
		friend struct FUserEvent;
		friend struct FWindowEvent;

		EEvent::TypeT Type;
		EEvent::SubTypeT SubType;
		Float32 TimeStamp;

		FEventInfo(
			const EEvent::TypeT InType,
			const EEvent::SubTypeT InSubType);

		FEventInfo(
			const EEvent::TypeT InType,
			const EEvent::SubTypeT InSubType,
			const Float32 InTimeStamp);

	private:
		FEventInfo() = default;
	};

	struct FEngineEvent
	{
		friend struct FEvent;

		FEventInfo Info;

	private:
		FEngineEvent() = default;
	};

	struct FGamePadEvent
	{
		friend struct FEvent;

		FEventInfo Info;
		EGamePadID::Value ID;
		EGamePadButton::Value Button;
		EInputAction::Value Action;
		EGamePadAxis::Value Axis;
		Float32 PrevAxis;
		Float32 CurrAxis;

		FGamePadEvent(
			const EGamePadEventType::Value SubType,
			const EGamePadID::Value ID,
			const Float32 TimeStamp);

		FGamePadEvent(
			const EGamePadID::Value ID,
			const EGamePadButton::Value Button,
			const EInputAction::Value Action,
			const Float32 TimeStamp);

		FGamePadEvent(
			const EGamePadID::Value ID,
			const EGamePadAxis::Value Axis,
			const Float32 PrevAxis,
			const Float32 CurrAxis,
			const Float32 TimeStamp);

	private:
		FGamePadEvent() = default;

		FGamePadEvent(
			const EGamePadEventType::Value SubType,
			const EGamePadID::Value InID,
			const EGamePadButton::Value InButton,
			const EInputAction::Value InAction,
			const EGamePadAxis::Value InAxis,
			const Float32 InPrevAxis,
			const Float32 InCurrAxis,
			const Float32 TimeStamp);
	};

	struct FKeyEvent
	{
		friend struct FEvent;

		FEventInfo Info;
		EKey::Value Key;
		EInputAction::Value Action;
		EKeyMods::BitMask Mods;

		FKeyEvent(
			const EKey::Value InKey,
			const EInputAction::Value InAction,
			const EKeyMods::BitMask InMods,
			const Float32 TimeStamp);

	private:
		FKeyEvent() = default;
	};

	struct FMovementAxisEvent
	{
		friend struct FEvent;

		FEventInfo Info;
		Float32 HorizontalAxis;
		Float32 VerticalAxis;

		FMovementAxisEvent(const Float32 HorizontalAxis, const Float32 VerticalAxis, const Float32 TimeStamp);

	private:
		FMovementAxisEvent() = default;
	};

	struct FMouseEvent
	{
		friend struct FEvent;

		FEventInfo Info;
		EMouseButton::Value Button;
		EInputAction::Value Action;
		EKeyMods::BitMask Mods;
		Float32 X;
		Float32 Y;
		EMouseEventInfo::Type MEInfo;

		FMouseEvent(
			const EMouseEventInfo::Type MEInfo,
			const Float32 TimeStamp);

		FMouseEvent(
			const EMouseEventType::Value SubType,
			const Float32 X,
			const Float32 Y,
			const Float32 TimeStamp);

		FMouseEvent(
			const EMouseButton::Value Button,
			const EInputAction::Value Action,
			const EKeyMods::BitMask Mods,
			const Float32 TimeStamp);

	private:
		FMouseEvent() = default;

		FMouseEvent(
			const EMouseEventType::Value SubType,
			const EMouseButton::Value InButton,
			const EInputAction::Value InAction,
			const EKeyMods::BitMask InMods,
			const Float32 InX,
			const Float32 InY,
			const EMouseEventInfo::Type InMEInfo,
			const Float32 TimeStamp);
	};

	struct FUpdateEvent
	{
		friend struct FEvent;

		FEventInfo Info;

		//Time is in seconds
		Float32 PreviousFrameTimeS;
		Float32 CurrentFrameTimeS;
		Float32 CurrentTimeS;
		Float32 DeltaTimeS;

		explicit FUpdateEvent(const Float32 TimeStamp);

	private:
		FUpdateEvent() = default;
	};

	struct FUserEvent
	{
		friend struct FEvent;

		FEventInfo Info;
		void* Ptr1;
		void* Ptr2;

		FUserEvent(
			const EEvent::SubTypeT SubType,
			void* const InPtr1,
			void* const InPtr2,
			const Float32 TimeStamp);

	private:
		FUserEvent() = default;
	};

	struct FWindowEvent
	{
		friend struct FEvent;

		typedef UInt16 ValueT;

		static const UInt8 UnsetValue;

		FEventInfo Info;
		ValueT X;
		ValueT Y;
		EWindowEventInfo::Type WEInfo;

		FWindowEvent(
			const EWindowEventType::Value SubType,
			const EWindowEventInfo::Type InWEInfo,
			const Float32 TimeStamp);

		FWindowEvent(
			const EWindowEventType::Value SubType,
			const ValueT InX,
			const ValueT InY,
			const Float32 TimeStamp);

	private:
		FWindowEvent() = default;

		FWindowEvent(
			const EWindowEventType::Value SubType,
			const ValueT InX,
			const ValueT InY,
			const EWindowEventInfo::Type InWEInfo,
			const Float32 TimeStamp);
	};

	struct FEvent
	{
		union
		{
			FEventInfo Info;
			FEngineEvent Engine;
			FGamePadEvent GamePad;
			FKeyEvent Key;
			FMovementAxisEvent MovementAxis;
			FMouseEvent Mouse;
			FUpdateEvent Update;
			FUserEvent User;
			FWindowEvent Window;
		};
	};

	static FOutputStream& operator<<(FOutputStream& LHS, const FEventInfo& RHS)
	{
		LHS << "@" << RHS.TimeStamp
			<< "s " << EEventType::ToString(RHS.Type)
			<< ": " << RHS.SubType << ". ";
		return LHS;
	}

	static FOutputStream& operator<<(FOutputStream& LHS, const FEngineEvent& RHS)
	{
		LHS << RHS.Info;
		return LHS;
	}

	static FOutputStream& operator<<(FOutputStream& LHS, const FGamePadEvent& RHS)
	{
		LHS << RHS.Info << "ID: " << RHS.ID << " ";

		static_assert(EGamePadEventType::Count == 3, "This function requires updating.");
		switch (RHS.Info.SubType)
		{
			case EGamePadEventType::Default:
			{
				if (RHS.Button != EGamePadButton::None)
				{
					LHS << "Btn: " << EGamePadButton::ToString(RHS.Button) << " " << EInputAction::ToString(RHS.Action);
				}
				else if (RHS.Axis != EGamePadAxis::None)
				{
					LHS << "Axis: " << EGamePadAxis::ToString(RHS.Axis) << " Prev { " << RHS.PrevAxis << " } New { " << RHS.CurrAxis << " }";
				}
				break;
			}

			case EGamePadEventType::Connected:
			{
				LHS << "Connected";
				break;
			}

			case EGamePadEventType::Disconnected:
			{
				LHS << "Disconnected";
				break;
			}

			default:
			{
				F_Assert(false, "Unsupported EGamePadEventType of " << RHS.Info.SubType);
				break;
			}
		}

		return LHS;
	}

	static FOutputStream& operator<<(FOutputStream& LHS, const FKeyEvent& RHS)
	{
		LHS << RHS.Info << "Key: " << static_cast<EKey::Type>(RHS.Key)
			<< " " << EInputAction::ToString(RHS.Action)
			<< " " << EKeyMods::ToString(RHS.Mods);
		return LHS;
	}

	static FOutputStream& operator<<(FOutputStream& LHS, const FMouseEvent& RHS)
	{
		LHS << RHS.Info << EMouseEventType::ToString(RHS.Info.SubType);

		static_assert(EMouseEventType::Count == 4, "This function requires updating.");
		switch (RHS.Info.SubType)
		{
			case EMouseEventType::Click:
			{
				LHS << ": " << RHS.Button
					<< " " << EInputAction::ToString(RHS.Action)
					<< " " << EKeyMods::ToString(RHS.Mods);
				break;
			}

			case EMouseEventType::Move:
			case EMouseEventType::Scroll:
			{
				LHS << ": { " << RHS.X << ", " << RHS.Y << " }";
				break;
			}

			case EMouseEventType::Misc:
			{
				LHS << ": { " << static_cast<Int64>(RHS.MEInfo) << " }";
				break;
			}

			default:
			{
				F_Assert(false, "Unsupported EMouseEventType of " << RHS.Info.SubType);
				break;
			}
		}

		return LHS;
	}

	static FOutputStream& operator<<(FOutputStream& LHS, const FUpdateEvent& RHS)
	{
		LHS << RHS.Info 
			<< "PreviousFrameT: " << RHS.PreviousFrameTimeS
			<< " CurrFrameT: " << RHS.CurrentFrameTimeS
			<< " CurrT: " << RHS.CurrentTimeS
			<< " DeltaT: " << RHS.DeltaTimeS;
		return LHS;
	}

	static FOutputStream& operator<<(FOutputStream& LHS, const FUserEvent& RHS)
	{
		LHS << RHS.Info
			<< "Ptrs { " << reinterpret_cast<SizeT>(RHS.Ptr1)
			<< ", " << reinterpret_cast<SizeT>(RHS.Ptr2) << " } ";
		return LHS;
	}

	static FOutputStream& operator<<(FOutputStream& LHS, const FWindowEvent& RHS)
	{
		LHS << RHS.Info << EWindowEventType::ToString(RHS.Info.SubType) << ": "
			<< "{ " << RHS.X << ", " << RHS.Y << " } { " << static_cast<Int64>(RHS.WEInfo) << " } ";

		return LHS;
	}

	static FOutputStream& operator<<(FOutputStream& LHS, const FEvent& RHS)
	{
		const std::streamsize DecimalCount = 3;
		LHS << std::fixed << std::setprecision(DecimalCount);

		switch (RHS.Info.Type)
		{
			case EEventType::Unknown:
			{
				LHS << RHS.Info;
				break;
			}

			case EEventType::Engine:
			{
				LHS << RHS.Engine;
				break;
			}

			case EEventType::GamePad:
			{
				LHS << RHS.GamePad;
				break;
			}

			case EEventType::Key:
			{
				LHS << RHS.Key;
				break;
			}

			case EEventType::Mouse:
			{
				LHS << RHS.Mouse;
				break;
			}

			case EEventType::Update:
			{
				LHS << RHS.Update;
				break;
			}

			case EEventType::User:
			{
				LHS << RHS.User;
				break;
			}

			case EEventType::Window:
			{
				LHS << RHS.Window;
				break;
			}

			default:
			{
				F_Assert(true, "Unsupported EEventType of " << RHS.Info.Type);
				break;
			}
		}

		return LHS;
	}
}

#endif
