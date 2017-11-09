#ifndef PHOENIX_S_MOVE_SIDEWAYS_H
#define PHOENIX_S_MOVE_SIDEWAYS_H

#include "Math/Math.h"
#include "Platform/Event/Event.h"
#include "Utility/Debug/Debug.h"

namespace Phoenix
{
	template<typename TRequirement>
	class SMoveSideways
	{
	public:
		template<typename TComponentManager>
		void Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
		{
			ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
				([](SizeT EntityID, CTransform& Transform, CRigidbody& Rigidbody, CMoveSideways& MoveSideways)
			{
				const Float32 PosX = Transform.Position.x;
				const bool PastSidewaysMoveLimit = FMathf::Abs(PosX) >= MoveSideways.MoveLimit;

				if (PastSidewaysMoveLimit)
				{
					Rigidbody.Velocity.x *= -1.0f;
				}

			});
		}
	};
}

#endif
