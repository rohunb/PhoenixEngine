#ifndef PHOENIX_S_PHYSICS_H
#define PHOENIX_S_PHYSICS_H

#include "Platform/Event/Event.h"
#include "EngineComponents/CTransform.h"
#include "EngineComponents/CRigidbody.h"
#include "Utility/Containers/Vector.h"

namespace Phoenix
{
	template<typename TRequirement>
	class SPhysics
	{
	public:
		template<typename TComponentManager>
		void Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
		{
			ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
				([DT = UpdateEvent.DeltaTimeS](SizeT EntityID, CTransform& Transform, CRigidbody& Rigidbody)
				{
					Rigidbody.Velocity += Rigidbody.Acceleration * DT;
					Transform.Position += Rigidbody.Velocity * DT;
				});
		}
	};

}
#endif
