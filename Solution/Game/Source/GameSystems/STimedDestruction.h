#ifndef PHOENIX_S_TIMED_DESTROY_GOLEM_H
#define PHOENIX_S_TIMED_DESTROY_GOLEM_H

#include "Platform/Event/Event.h"

namespace Phoenix
{
	template<typename TRequirement>
	class STimedDestruction
	{
	public:
		template<typename TComponentManager>
		void Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
		{
			ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
				([DT = UpdateEvent.DeltaTimeS, &ComponentManager]
					(SizeT EntityID, CTimedDestruction& TimedDestruction)
			{
				if (TimedDestruction.CurrentTimeSeconds >= TimedDestruction.TimeUntilDestructionSeconds)
				{
					TimedDestruction.CurrentTimeSeconds = 0.0f;
					ComponentManager.Destroy(EntityID);
				}
				else
				{
					TimedDestruction.CurrentTimeSeconds += DT;
				}
			});
		}
	};

}

#endif
