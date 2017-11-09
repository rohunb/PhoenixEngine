#ifndef PHOENIX_COMPONENT_MANAGER_IMPL_H
#define PHOENIX_COMPONENT_MANAGER_IMPL_H

#include "Config/ECS/ComponentsRegistry.h"
#include "ECS/ComponentManager.h"

namespace Phoenix
{
	struct FComponentManagerImpl
	{
		TComponentManager<FComponentRegistry::Config> ComponentManager;
	};
}

#endif
