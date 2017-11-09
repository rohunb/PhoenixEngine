#pragma once

#include <string>
#include <vector>

#include "Utility/Misc/ClassTypeId.h"

#define F_GeneratedComponentClass(Class) \
Class() : Component() { \
}

namespace Phoenix
{
	class BaseComponent
	{
	public:
		class FGameObject* Parent{ nullptr };
		class ComponentStorage* Storage{ nullptr };
		class FGameScene* GameScene{ nullptr };

		BaseComponent() = default;
		virtual ~BaseComponent() = default;
		virtual void Init() = 0;
	};

	template<typename T>
	class Component
		: public BaseComponent
	{
	public:

		static TypeId GetTypeId()
		{
			return ClassTypeId<BaseComponent>::GetTypeId<T>();
		}

		// Each component must update each loop
		virtual void Update(float dt)
		{
		}
	};
	typedef std::vector<std::reference_wrapper<BaseComponent>> ComponentArray;
}