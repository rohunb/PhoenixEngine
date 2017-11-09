#ifndef PHOENIX_S_MOVE_ON_INPUT_H
#define PHOENIX_S_MOVE_ON_INPUT_H

#include "Platform/Event/Event.h"
#include "Platform/Input/Keys.h"
#include "Utility/Debug/Assert.h"

namespace Phoenix
{
	template<typename TRequirement>
	class SMoveOnInput
	{
	public:
		template<typename TComponentManager>
		void Init(TComponentManager& ComponentManager);

	private:
		template<typename TComponentManager>
		void OnMovementAxisCallback(const FMovementAxisEvent& MoveAxisEvent, TComponentManager& CompomentManager);
	};

	template<typename TRequirement>
	template<typename TComponentManager>
	void SMoveOnInput<TRequirement>::Init(TComponentManager& ComponentManager)
	{
		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([this, &ComponentManager](SizeT EntityID, CMoveSpeed&, CRigidbody&, CInput& Input)
		{
			Input.MovementAxisCallback = [this, &ComponentManager](const FMovementAxisEvent& MoveAxisEvent)
			{
				F_Assert(this, "System has been deleted");
				OnMovementAxisCallback(MoveAxisEvent, ComponentManager);
			};
		});
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SMoveOnInput<TRequirement>::OnMovementAxisCallback(const FMovementAxisEvent& MoveAxisEvent, TComponentManager& CompomentManager)
	{
		CompomentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([&MoveAxisEvent](SizeT EntityID, CMoveSpeed& MoveSpeed, CRigidbody& Rigidbody, CInput&)
		{
			const FVector3D MoveDir{ MoveAxisEvent.HorizontalAxis, MoveAxisEvent.VerticalAxis, 0.0f };
			Rigidbody.Velocity = MoveDir * MoveSpeed.MoveSpeed;
		});
	}
}

#endif
