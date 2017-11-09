#ifndef PHOENIX_INPUT_ACTION_H
#define PHOENIX_INPUT_ACTION_H

#include "ExternalLib/GLFWIncludes.h"
#include "Utility/Containers/Array.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	namespace EInputAction
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			Release = GLFW_RELEASE, 
			Press = GLFW_PRESS,
			Repeat = GLFW_REPEAT,
			None,
			Count
		};

		static bool IsValid(const Type InputAction)
		{
			const bool Result = InputAction < EInputAction::Count;
			return Result;
		}

		static const FChar* ToString(const Type InputAction)
		{
			static_assert(EInputAction::Count == 4, "This table requires updating.");
			static const TArray<const FChar* const, EInputAction::Count> LookUpTable = {
				"Up",
				"Down",
				"Held",
				"None"
			};

			const FChar* const Result = LookUpTable[InputAction];
			return Result;
		}
	}
}
#endif
