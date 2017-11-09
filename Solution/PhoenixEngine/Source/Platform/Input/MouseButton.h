#ifndef PHOENIX_MOUSE_BUTTON_H
#define PHOENIX_MOUSE_BUTTON_H

#include "ExternalLib/GLFWIncludes.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	namespace EMouseButton
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			Left = GLFW_MOUSE_BUTTON_LEFT,
			Right = GLFW_MOUSE_BUTTON_RIGHT,
			Middle = GLFW_MOUSE_BUTTON_MIDDLE,
			Btn1 = GLFW_MOUSE_BUTTON_1,
			Btn2 = GLFW_MOUSE_BUTTON_2,
			Btn3 = GLFW_MOUSE_BUTTON_3,
			Btn4 = GLFW_MOUSE_BUTTON_4,
			Btn5 = GLFW_MOUSE_BUTTON_5,
			Btn6 = GLFW_MOUSE_BUTTON_6,
			Btn7 = GLFW_MOUSE_BUTTON_7,
			Btn8 = GLFW_MOUSE_BUTTON_8,
			Last = GLFW_MOUSE_BUTTON_LAST,
			Unknown,
			Count
		};

		static bool IsValid(const Type MouseButton)
		{
			const bool Result = MouseButton < EMouseButton::Count;
			return Result;
		}
	}
}

#endif
