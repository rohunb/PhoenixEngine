#ifndef PHOENIX_S_RANDOM_SPIN_H
#define PHOENIX_S_RANDOM_SPIN_H

#include "Math/Quaternion.h"
#include "Platform/Event/Event.h"
#include "Utility/Misc/Random.h"

namespace Phoenix
{
	template<typename TRequirement>
	class SRandomSpin
	{
	public:
		template<typename TComponentManager>
		void OnEntityCreated(SizeT EntityID, TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager);
	};

	template<typename TRequirement>
	template<typename TComponentManager>
	void SRandomSpin<TRequirement>::OnEntityCreated(SizeT EntityID, TComponentManager& ComponentManager)
	{
		const bool EntityMeetsRequirement = ComponentManager.template MeetsRequirement<TRequirement>(EntityID);
		if (EntityMeetsRequirement)
		{
			CRandomSpin& RandomSpin = ComponentManager.template GetComponent<CRandomSpin>(EntityID);

			FRandom Random;

			const Float32 RandomRotationSpeed = Random.Range(-RandomSpin.RotationSpeed, RandomSpin.RotationSpeed);
			const FVector3D RandomRotationAxis = Random.UnitVector3();

			RandomSpin.RotationSpeed = RandomRotationSpeed;
			RandomSpin.RotationAxis = RandomRotationAxis;
		}
	}
	

	template<typename TRequirement>
	template<typename TComponentManager>
	void SRandomSpin<TRequirement>::Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
	{
		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([DT = UpdateEvent.DeltaTimeS]
			 (SizeT EntityID, CTransform& Transform, CRandomSpin& RandomSpin)
			{
				const Float32 AngleToRotateBy = RandomSpin.RotationSpeed * DT;
				const FQuaternion Rotation = glm::angleAxis(AngleToRotateBy, RandomSpin.RotationAxis);

				Transform.Rotation = Rotation * Transform.Rotation;
			});
	}

}

#endif