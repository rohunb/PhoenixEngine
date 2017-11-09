#ifndef PHOENIX_C_MOVE_SIDEWAYS_H
#define PHOENIX_C_MOVE_SIDEWAYS_H

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	struct CMoveSideways
	{
		Float32 MoveLimit { 0.5f };

		CMoveSideways() = default;

		explicit CMoveSideways(Float32 MoveLimit)
			: MoveLimit(MoveLimit)
		{}
	};
}

#endif
