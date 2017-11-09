#ifndef PHOENIX_C_GOLEM_DESTROY_TIME_H
#define PHOENIX_C_GOLEM_DESTROY_TIME_H

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	struct CTimedDestruction
	{
		const Float32 TimeUntilDestructionSeconds{ 1.0f };

		Float32 CurrentTimeSeconds{ 0.0f };

		CTimedDestruction() = default;

		explicit CTimedDestruction(Float32 TimeUntilDestructionSeconds)
			: TimeUntilDestructionSeconds(TimeUntilDestructionSeconds)
		{
		}
	};
}

#endif
