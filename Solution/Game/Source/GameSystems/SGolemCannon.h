#ifndef PHOENIX_S_GOLEM_CANNON_H
#define PHOENIX_S_GOLEM_CANNON_H

#include "Platform/Event/Event.h"

namespace Phoenix
{
	template<typename TRequirement>
	class SGolemCannon
	{
	public:
		template<typename TComponentManager>
		void Init(TComponentManager& ComponentManager);

	private:
		template<typename TComponentManager>
		void Shoot(const FKeyEvent& KeyEvent, TComponentManager& ComponentManager, SizeT CannonEntityID);
	};

	template<typename TRequirement>
	template<typename TComponentManager>
	void SGolemCannon<TRequirement>::Init(TComponentManager& ComponentManager)
	{
		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([this, &ComponentManager](SizeT EntityID, CGolemCannon&, CTransform&, CInput& Input)
		{
			Input.KeyCallback = [this, &ComponentManager, EntityID](const FKeyEvent& KeyEvent)
			{
				F_Assert(this, "SGolemCannon does not exist");

				const bool ShouldShoot = KeyEvent.Key == EKey::Space && KeyEvent.Action == EInputAction::Press;
				if (ShouldShoot)
				{
					Shoot(KeyEvent, ComponentManager, EntityID);
				}
			};
		});
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SGolemCannon<TRequirement>::Shoot(const FKeyEvent& KeyEvent, TComponentManager& ComponentManager, SizeT CannonEntityID)
	{
		const CTransform& CannonTransform = ComponentManager.template GetComponent<CTransform>(CannonEntityID);
		const CGolemCannon& GolemCannon = ComponentManager.template GetComponent<CGolemCannon>(CannonEntityID);

		const FVector3D& CannonPosition = CannonTransform.Position;
		const FVector3D CannonBallScale{ 0.35f };
		const FVector3D CannonBallVelocity = GolemCannon.ShootDirection * GolemCannon.ShootSpeed;

		const SizeT CannonBall = ComponentManager.CreateEntity();
		ComponentManager.template AddComponent<CModel>(CannonBall, "golem.pmesh");
		ComponentManager.template AddComponent<CTransform>(CannonBall, CannonPosition, CannonBallScale);
		ComponentManager.template AddComponent<CRigidbody>(CannonBall, CannonBallVelocity);
		ComponentManager.template AddComponent<CTimedDestruction>(CannonBall, 2.0f);
		ComponentManager.template AddComponent<CRandomSpin>(CannonBall);
	}
}

#endif
