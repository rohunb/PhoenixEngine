#ifndef PHOENIX_S_TIMED_SPAWN_GOLEM_H
#define PHOENIX_S_TIMED_SPAWN_GOLEM_H

#include "Math/Math.h"
#include "Platform/Event/Event.h"
#include "Utility/Misc/Random.h"

namespace Phoenix
{
	template<typename TRequirement>
	class STimedSpawnGolem
	{
	public:
		template<typename TComponentManager>
		void Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
		{
			ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
				([DT = UpdateEvent.DeltaTimeS, &ComponentManager, this](SizeT EntityID, CGolemSpawnTime& TimedSpawnGolem) 
				{
					if (TimedSpawnGolem.CurrentTimeSeconds >= TimedSpawnGolem.SpawnIntervalSeconds)
					{
						SpawnGolem(ComponentManager);
						TimedSpawnGolem.CurrentTimeSeconds = 0.0f;
					}

					TimedSpawnGolem.CurrentTimeSeconds += DT;
				});
		}

	private:
		template<typename TComponentManager>
		void SpawnGolem(TComponentManager& ComponentManager)
		{
			SizeT GolemEntity = ComponentManager.CreateEntity();
			
			ComponentManager.template AddComponent<CModel>(GolemEntity, "golem.pmesh");

			FRandom Random;
			
			const FVector3D RandomPosition = Random.UnitVector3() * 3.0f;
			const Float32 RandomVelocityX = Random.Range(-3.0f, 3.0f);

			ComponentManager.template AddComponent<CTransform>(GolemEntity, RandomPosition);
			ComponentManager.template AddComponent<CRigidbody>(GolemEntity, FVector3D(RandomVelocityX, 0.0f, 0.0f));
			
			ComponentManager.template AddComponent<CMoveSideways>(GolemEntity, 1.5f);
			
			ComponentManager.template AddComponent<CTimedDestruction>(GolemEntity);
		}
	};
}

#endif
