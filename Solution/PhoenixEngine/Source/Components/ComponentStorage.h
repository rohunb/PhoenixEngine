#pragma once
#include <vector>
#include <memory>

#include "GameObject/GameObject.h"
#include "Components/Component.h"
#include "Components/ComponentTypeArray.h"

namespace Phoenix
{
	class ComponentStorage
	{
	public:
		ComponentStorage(std::size_t InGameObjectAmount);
		~ComponentStorage();

		ComponentStorage(const ComponentStorage&) = delete;
		ComponentStorage& operator=(const ComponentStorage&) = delete;

		ComponentStorage(ComponentStorage&&) = delete;
		ComponentStorage& operator=(ComponentStorage&&) = delete;

		void AddComponent(FGameObject* InGameObject, BaseComponent* InComponent, TypeId InComponentTypeId);

		BaseComponent& GetComponent(const FGameObject& InGameObject, TypeId InTypeId);

		BaseComponent& GetComponentById(const FGameObject::ID::IntType& InGameObjectId, const std::size_t& InComponentId);

		ComponentTypeArray GetComponentTypes(const FGameObject& InGameObject) const;

		void RemoveComponent(const FGameObject& InGameObject, TypeId InTypeId);

		void RemoveAllComponents(FGameObject& InGameObject);

		void Resize(std::size_t InAmount);
	private:
		typedef std::vector<std::unique_ptr<BaseComponent> > ImplComponentArray;

		struct GameObjectComponents
		{
			GameObjectComponents() = default;

			GameObjectComponents(GameObjectComponents&& E) :
				Components(std::move(E.Components)),
				ComponentTypeList(std::move(E.ComponentTypeList))
			{
			}
			ImplComponentArray Components;

			ComponentTypeArray ComponentTypeList;
		};

		std::vector<GameObjectComponents> ComponentEntries;

		ImplComponentArray& GetComponents_Implementation(const FGameObject& E);

		const ImplComponentArray& GetComponents_Implementation(const FGameObject& E) const;
	};
}