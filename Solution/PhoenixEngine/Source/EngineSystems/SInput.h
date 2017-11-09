#ifndef PHOENIX_S_INPUT_H
#define PHOENIX_S_INPUT_H

#include "Platform/Event/Event.h"

namespace Phoenix
{
	template<typename TRequirement>
	class SInput
	{
	public:
		template<typename TComponentManager>
		void OnEvent(const FEvent& Event, TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void DeInit(TComponentManager& ComponentManager);

	private:
		template<typename TComponentManager>
		void OnMovementAxisEvent(const FMovementAxisEvent& MoveAxisEvent, TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void OnKeyEvent(const FKeyEvent& KeyEvent, TComponentManager& ComponentManager);
	};


	template<typename TRequirement>
	template<typename TComponentManager>
	void SInput<TRequirement>::OnEvent(const FEvent& Event, TComponentManager& ComponentManager)
	{
		switch (Event.Info.Type)
		{
			case EEventType::Key:
			{
				OnKeyEvent(Event.Key, ComponentManager);
				break;
			}
			case EEventType::MovementAxis:
			{
				OnMovementAxisEvent(Event.MovementAxis, ComponentManager);
				break;
			}
			default:
				break;
		}
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SInput<TRequirement>::DeInit(TComponentManager& ComponentManager)
	{
		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([](SizeT EntityID, CInput& Input) 
			{
				Input.ClearAllCallbacks();
			});
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SInput<TRequirement>::OnMovementAxisEvent(const FMovementAxisEvent& MoveAxisEvent, TComponentManager& ComponentManager)
	{
		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([&MoveAxisEvent](SizeT EntityID, CInput& Input)
			{
				if (Input.MovementAxisCallback)
				{
					Input.MovementAxisCallback(MoveAxisEvent);
				}
			});
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SInput<TRequirement>::OnKeyEvent(const FKeyEvent& KeyEvent, TComponentManager& ComponentManager)
	{
		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([&KeyEvent](SizeT EntityID, CInput& Input) 
			{
				if (Input.KeyCallback)
				{
					Input.KeyCallback(KeyEvent);
				}
			});
	}


}

#endif
