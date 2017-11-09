#ifndef PHOENIX_C_TIMED_SPAWN_GOLEM_H
#define PHOENIX_C_TIMED_SPAWN_GOLEM_H

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	struct CGolemSpawnTime
	{
		const Float32 SpawnIntervalSeconds{ 0.5f };

		Float32 CurrentTimeSeconds{ 0.0f };
	};
}

#endif
