#include "Stdafx.h"
#include "Components/ComponentStorage.h"
#include "Components/ComponentTypeArray.h"
#include <iostream>

#include <memory>

using namespace Phoenix;

ComponentStorage::ComponentStorage(std::size_t InGameObjectAmount) :
	ComponentEntries(InGameObjectAmount)
{
}


ComponentStorage::~ComponentStorage()
{
}

void ComponentStorage::AddComponent(FGameObject* InGameObject, BaseComponent* InComponent, TypeId InComponentTypeId)
{
	auto Index = InGameObject->GetId().Index;

	auto& ComponentData = ComponentEntries[Index];

	CheckCapacity(ComponentData.Components, InComponentTypeId);
	ComponentData.Components[InComponentTypeId].reset(InComponent);

	CheckCapacity(ComponentData.ComponentTypeList, InComponentTypeId);
	ComponentData.ComponentTypeList[InComponentTypeId] = true;

	ComponentData.Components[InComponentTypeId]->Storage = this;
	ComponentData.Components[InComponentTypeId]->Parent = InGameObject;
	ComponentData.Components[InComponentTypeId]->Init();
}

ComponentTypeArray ComponentStorage::GetComponentTypes(const FGameObject& InGameObject) const
{
	return ComponentEntries[InGameObject.GetId().Index].ComponentTypeList;
}

void ComponentStorage::RemoveComponent(const FGameObject& InGameObject, TypeId InTypeId)
{
	auto Index = InGameObject.GetId().Index;
	auto& ComponentData = ComponentEntries[Index];

	ComponentData.Components[InTypeId].reset();
	ComponentData.ComponentTypeList[InTypeId] = false;
}

void ComponentStorage::RemoveAllComponents(FGameObject& InGameObject)
{
	auto Index = InGameObject.GetId().Index;
	auto& ComponentData = ComponentEntries[Index];

	ComponentData.Components.clear();
	ComponentData.ComponentTypeList.clear();
}

void ComponentStorage::Resize(std::size_t InAmount)
{
	ComponentEntries.resize(InAmount);
}

BaseComponent& ComponentStorage::GetComponent(const FGameObject& InGameObject, TypeId InTypeId)
{
	return *GetComponents_Implementation(InGameObject)[InTypeId];
}

BaseComponent& ComponentStorage::GetComponentById(const FGameObject::ID::IntType& InGameObjectId, const std::size_t& InComponentId)
{
	return *ComponentEntries[InGameObjectId].Components[InComponentId];
}

ComponentStorage::ImplComponentArray& ComponentStorage::GetComponents_Implementation(const FGameObject& E)
{
	return ComponentEntries[E.GetId().Index].Components;
}

const ComponentStorage::ImplComponentArray& ComponentStorage::GetComponents_Implementation(const FGameObject& E) const
{
	return ComponentEntries[E.GetId().Index].Components;
}
