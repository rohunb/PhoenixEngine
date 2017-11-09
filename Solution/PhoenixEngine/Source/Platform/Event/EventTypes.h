#ifndef PHOENIX_EVENT_TYPES_H
#define PHOENIX_EVENT_TYPES_H

#include "Utility/Containers/Array.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	namespace EEvent
	{
		typedef UInt16 TypeT;
		typedef UInt16 SubTypeT;
	}

	namespace EEventType
	{
		typedef EEvent::TypeT Type;

		enum Value : Type
		{
			Unknown,
			Engine,
			GamePad,
			Key,
			MovementAxis,
			Mouse,
			Update,
			User,
			Window,
			Count
		};

		static const FChar* ToString(const Type EventType)
		{
			static_assert(EEventType::Count == 9, "This table requires updating.");
			static const TArray<const FChar* const, EEventType::Count> LookUpTable = 
			{
				"Unknown",
				"Engine",
				"GamePad",
				"Key",
				"MovementAxis",
				"Mouse",
				"Update",
				"User",
				"Window",
			};

			const FChar* const Result = LookUpTable[EventType];
			return Result;
		}
	}

	namespace ECursorEventType
	{
		typedef EEvent::SubTypeT Type;

		enum Value : Type
		{
			Enter,
			Leave,
			Position,
			Count
		};

		static const FChar* ToString(const Type CursorEventType)
		{
			static_assert(ECursorEventType::Count == 3, "This table requires updating.");
			static const TArray<const FChar* const, ECursorEventType::Count> LookUpTable =
			{
				"Enter",
				"Leave",
				"Position",
			};

			const FChar* const Result = LookUpTable[CursorEventType];
			return Result;
		}
	}

	namespace EEngineEventType
	{
		typedef EEvent::SubTypeT Type;

		enum Value : Type
		{
			ShutDownNow,
			IsShutDown,
			Count
		};

		static const FChar* ToString(const Type EngineEventType)
		{
			static_assert(EEngineEventType::Count == 2, "This table requires updating.");
			static const TArray<const FChar* const, EEngineEventType::Count> LookUpTable =
			{
				"ShutDownNow",
				"IsShutDown"
			};

			const FChar* const Result = LookUpTable[EngineEventType];
			return Result;
		}
	}

	namespace EGamePadEventType
	{
		typedef EEvent::SubTypeT Type;

		enum Value : Type
		{
			Default,
			Connected,
			Disconnected,
			Count
		};

		static const FChar* ToString(const Type GamePadEventType)
		{
			static_assert(EGamePadEventType::Count == 3, "This table requires updating.");
			static const TArray<const FChar* const, EGamePadEventType::Count> LookUpTable =
			{
				"Default",
				"Connected",
				"Disconnected"
			};

			const FChar* const Result = LookUpTable[GamePadEventType];
			return Result;
		}
	}

	namespace EKeyEventType
	{
		typedef EEvent::SubTypeT Type;

		enum Value : Type
		{
			Default,
			Count
		};

		static const FChar* ToString(const Type KeyEventType)
		{
			static_assert(EKeyEventType::Count == 1, "This table requires updating.");
			static const TArray<const FChar* const, EKeyEventType::Count> LookUpTable =
			{
				"Default",
			};

			const FChar* const Result = LookUpTable[KeyEventType];
			return Result;
		}
	}

	namespace EMovementAxisEventType
	{
		typedef EEvent::SubTypeT Type;

		enum Value : Type
		{
			Default,
			Count
		};

		static const FChar* ToString(const Type MovementAxisEventType)
		{
			static_assert(EMovementAxisEventType::Count == 1, "This table requires updating.");
			static const TArray<const FChar* const, EMovementAxisEventType::Count> LookUpTable =
			{
				"Default",
			};

			const FChar* const Result = LookUpTable[MovementAxisEventType];
			return Result;
		}
	}

	namespace EMouseEventType
	{
		typedef EEvent::SubTypeT Type;

		enum Value : Type
		{
			Click,
			Move,
			Scroll,
			Misc,
			Count
		};

		static const FChar* ToString(const Type MouseEventType)
		{
			static_assert(EMouseEventType::Count == 4, "This table requires updating.");
			static const TArray<const FChar* const, EMouseEventType::Count> LookUpTable =
			{
				"Click",
				"Move",
				"Scroll",
				"Misc"
			};

			const FChar* const Result = LookUpTable[MouseEventType];
			return Result;
		}
	}

	namespace EUpdateEventType
	{
		typedef EEvent::SubTypeT Type;

		enum Value : Type
		{
			Default,
			Count
		};

		static const FChar* ToString(const Type KeyEventType)
		{
			static_assert(EUpdateEventType::Count == 1, "This table requires updating.");
			static const TArray<const FChar* const, EUpdateEventType::Count> LookUpTable =
			{
				"Default",
			};

			const FChar* const Result = LookUpTable[KeyEventType];
			return Result;
		}
	}

	namespace EWindowEventType
	{
		typedef EEvent::SubTypeT Type;

		enum Value : Type
		{
			CloseAttempt,
			Focus,
			FrameBufferResized,
			Restored,

			Iconified,
			PositionChanged,
			Refreshed,
			SizeChanged,
			Count
		};

		static const FChar* ToString(const Type WindowEventType)
		{
			static_assert(EWindowEventType::Count == 8, "This table requires updating.");
			static const TArray<const FChar* const, EWindowEventType::Count> LookUpTable =
			{
				"CloseAttempt",
				"Focus",
				"FrameBufferResized",
				"Restored",

				"Iconified",
				"PositionChanged",
				"Refreshed",
				"SizeChanged"
			};

			const FChar* const Result = LookUpTable[WindowEventType];
			return Result;
		}
	}
}

#endif
