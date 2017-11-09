#ifndef PHOENIX_C_INPUT_H
#define PHOENIX_C_INPUT_H

#include "Platform/Event/Event.h"
#include "Utility/Misc/Function.h"

namespace Phoenix
{
	struct CInput
	{
		//#FIXME Finalize Input Design
		TFunction<void(const FKeyEvent&)> KeyCallback;
		TFunction<void(const FMovementAxisEvent&)> MovementAxisCallback;

		void ClearAllCallbacks();
	};
}

#endif
