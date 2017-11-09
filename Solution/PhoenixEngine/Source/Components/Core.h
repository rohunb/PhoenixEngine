#pragma once
#include <vector>

#include "Components/ComponentFilter.h"
#include "GameObject/GameObject.h"
#include "Utility/Misc/ClassTypeId.h"
#include "Utility/Debug/Assert.h"
#include "Utility/Debug/Debug.h"

namespace Phoenix
{
	class FGameScene;

	class BaseCore
	{
	public:
		BaseCore() = default;
		BaseCore(const Phoenix::ComponentFilter& Filter);
		virtual ~BaseCore() = 0;

		// Each core must update each loop
		virtual void Update(const struct FUpdateEvent& UpdateEvent) = 0;

		// Get The GameScene attached to the Core
		FGameScene& GetGameScene() const;

		// Get All the entities that are within the Core
		std::vector<FGameObject> GetEntities() const;

		// Get the component filter associated with the core.
		const ComponentFilter& GetComponentFilter() const;
	protected:
		class Engine* GameEngine;
	private:
		// Separate init from construction code.
		virtual void Init() = 0;
		virtual void DeInit() = 0;

		// Add an GameObject to the core
		void Add(FGameObject& InGameObject);

		void Remove(FGameObject& InGameObject);

		// The Entities that are attached to this system
		std::vector<FGameObject> Entities;

		// The GameScene attached to the system
		FGameScene* GameScene;

		ComponentFilter CompFilter;

		friend class FGameScene;
	};

	// Use the CRTP patten to define custom systems
	template<typename T>
	class FCore
		: public BaseCore
	{
	public:
		typedef FCore<T> Base;

		FCore() = default;

		FCore(const ComponentFilter& InComponentFilter) : BaseCore(InComponentFilter)
		{

		}

		static TypeId GetTypeId()
		{
			return ClassTypeId<BaseCore>::GetTypeId<T>();
		}
	};
}

