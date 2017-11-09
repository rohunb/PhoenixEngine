#ifndef PHOENIX_GAME_PAD_H
#define PHOENIX_GAME_PAD_H

#include "ExternalLib/GLFWIncludes.h"
#include "Utility/Containers/Array.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	namespace EGamePadAxis
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			LeftThumbX,
			LeftThumbY,

			LeftTrigger,

			RightThumbStickY,
			RightThumbStickX,

			Unknown1,
			Unknown2,
			Unknown3,
			Unknown4,
			Unknown5,

			None,
			Count
		};

		static const FChar* ToString(const Value GamePadAxis)
		{
			static_assert(EGamePadAxis::Count == 11, "This table requires updating.");
			static const TArray<const FChar* const, EGamePadAxis::Count> LookUpTable =
			{
				"LeftThumbX",
				"LeftThumbY",

				"LeftTrigger",

				"RightThumbStickY",
				"RightThumbStickX",

				"Unknown1",
				"Unknown2",
				"Unknown3",
				"Unknown4",
				"Unknown5",

				"None",
			};

			const FChar* const Result = LookUpTable[GamePadAxis];
			return Result;
		}
	}

	namespace EGamePadButton
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			A,
			B,
			X,
			Y,

			LeftBumper,
			RightBumper,

			Select,
			Start,

			LeftThumbStick,
			RightThumbStick,

			DPadUp,
			DPadRight,
			DPadDown,
			DPadLeft,

			Unknown1,
			Unknown2,
			Unknown3,
			Unknown4,
			Unknown5,

			None,

			Count
		};

		static const FChar* ToString(const Value GamePadButton)
		{
			static_assert(EGamePadButton::Count == 20, "This table requires updating.");
			static const TArray<const FChar*, EGamePadButton::Count> LookUpTable =
			{
				"A",
				"B",
				"X",
				"Y",

				"LeftBumper",
				"RightBumper",

				"Select",
				"Start",

				"LeftThumbStick",
				"RightThumbStick",

				"DPadUp",
				"DPadRight",
				"DPadDown",
				"DPadLeft",

				"Unknown1",
				"Unknown2",
				"Unknown3",
				"Unknown4",
				"Unknown5",

				"None",
			};


			const FChar* const Result = LookUpTable[GamePadButton];
			return Result;
		}
	}

	namespace EGamePadID
	{
		typedef Int32 Type;

		enum Value : Type
		{
			GP1 = GLFW_JOYSTICK_1,
			GP2 = GLFW_JOYSTICK_2,
			GP3 = GLFW_JOYSTICK_3,
			GP4 = GLFW_JOYSTICK_4,
			GP5 = GLFW_JOYSTICK_5,

			GP6 = GLFW_JOYSTICK_6,
			GP7 = GLFW_JOYSTICK_7,
			GP8 = GLFW_JOYSTICK_8,
			GP9 = GLFW_JOYSTICK_9,
			GP10 = GLFW_JOYSTICK_10,

			GP11 = GLFW_JOYSTICK_11,
			GP12 = GLFW_JOYSTICK_12,
			GP13 = GLFW_JOYSTICK_13,
			GP14 = GLFW_JOYSTICK_14,
			GP15 = GLFW_JOYSTICK_15,

			GP16 = GLFW_JOYSTICK_16,
			Last = GLFW_JOYSTICK_LAST,
			Count,
			Unknown = Count
		};
	}
}

#endif
