#ifndef PHOENIX_SYSTEM_TEMPLATE_H
#define PHOENIX_SYSTEM_TEMPLATE_H

#include "Platform/Event/Event.h"

namespace Phoenix
{
	class STemplate
	{
		/* If any of the following methods are defined in a System, they will be called automatically.
		*  Ensure that the methods are PUBLIC. They will not be invoked automatically otherwise.
		*/
	
	public:

		template<typename TComponentManager>
		void Init(TComponentManager& ComponentManager)
		{}

		template<typename TComponentManager>
		void DeInit(TComponentManager& ComponentManager)
		{}

		template<typename TComponentManager>
		void Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
		{}

		template<typename TComponentManager>
		void OnEvent(const FEvent& Event, TComponentManager& ComponentManager)
		{}

		template<typename TComponentManager>
		void OnEntityCreated(SizeT NewEntity, TComponentManager& ComponentManager)
		{}

		template<typename TComponentManager>
		void OnEntityDestroyed(SizeT DestroyedEntity, TComponentManager& ComponentManager)
		{}

	};
}

#endif
