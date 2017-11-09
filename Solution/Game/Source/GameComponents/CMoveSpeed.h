#ifndef PHOENIX_C_MOVEMENT_H
#define PHOENIX_C_MOVEMENT_H

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	struct CMoveSpeed
	{
		Float32 MoveSpeed{ 0.0f };

		CMoveSpeed() = default;

		explicit CMoveSpeed(Float32 MoveSpeed)
			: MoveSpeed(MoveSpeed)
		{
		}
	};
}

#endif
