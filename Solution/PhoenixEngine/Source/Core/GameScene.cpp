#include "Stdafx.h"
#include "Core/GameScene.h"

using namespace Phoenix;

#define DEFAULT_ENTITY_POOL_SIZE 50

FGameScene::FGameScene()
	: ObjectIdPool(DEFAULT_ENTITY_POOL_SIZE),
	GameObjectAttributes(DEFAULT_ENTITY_POOL_SIZE)
{

}

FGameScene::~FGameScene()
{
}

void FGameScene::Init(FComponentManager& ComponentManager)
{
	OnInit(ComponentManager);
}

void FGameScene::Update(const FUpdateEvent& UpdateEvent, FComponentManager& ComponentManager)
{
	SimulateGameObjects();
	OnUpdate(UpdateEvent, ComponentManager);
}

void FGameScene::DeInit()
{
	OnDeInit();
}

FGameObject& FGameScene::CreateGameObject()
{
	CheckForResize(1);
	GameObjectCache.Alive.emplace_back(*this, ObjectIdPool.Create());
	GameObjectCache.Alive.back().SetActive(true);
	return GameObjectCache.Alive.back();
}

void FGameScene::SimulateGameObjects()
{
	for (auto& InGameObject : GameObjectCache.Activated)
	{
		auto& Attr = GameObjectAttributes.Attributes[InGameObject.GetId().Index];
		Attr.IsActive = true;

		for (auto& InCore : Cores)
		{
			auto CoreIndex = InCore.first;

			if (InCore.second->GetComponentFilter().PassFilter(GameObjectAttributes.Storage.GetComponentTypes(InGameObject)))
			{
				if (Attr.Cores.size() <= CoreIndex || !Attr.Cores[CoreIndex])
				{
					InCore.second->Add(InGameObject);

					CheckCapacity(Attr.Cores, CoreIndex);
					Attr.Cores[CoreIndex] = true;
				}
			}
			else if (Attr.Cores.size() > CoreIndex && Attr.Cores[CoreIndex])
			{
				InCore.second->Remove(InGameObject);
				Attr.Cores[CoreIndex] = false;
			}
		}
	}
	for (auto& InGameObject : GameObjectCache.Deactivated)
	{
		auto& Attr = GameObjectAttributes.Attributes[InGameObject.GetId().Index];
		Attr.IsActive = false;

		for (auto& InCore : Cores)
		{
			auto CoreIndex = InCore.first;
			if (Attr.Cores.size() <= CoreIndex) continue;
			if (Attr.Cores[CoreIndex])
			{
				InCore.second->Remove(InGameObject);
				Attr.Cores[CoreIndex] = false;
			}
		}
	}

	for (auto& InGameObject : GameObjectCache.Killed)
	{

		for (auto e = GameObjectCache.Alive.begin(); e != GameObjectCache.Alive.end(); ++e)
		{
			if (*e == InGameObject)
			{
				GameObjectCache.Alive.erase(e);
				return;
			}
		}
		GameObjectAttributes.Storage.RemoveAllComponents(InGameObject);

		ObjectIdPool.Remove(InGameObject.GetId());
	}

	GameObjectCache.ClearTemp();
}

void FGameScene::AddCore(BaseCore& InCore, TypeId InCoreTypeId)
{
	Cores[InCoreTypeId].reset(&InCore);
	InCore.GameScene = this;
	InCore.Init();
}

void FGameScene::CheckForResize(std::size_t InNumEntitiesToBeAllocated)
{
	auto NewSize = GetGameObjectCount() + InNumEntitiesToBeAllocated;

	if (NewSize > ObjectIdPool.GetSize())
	{
		Resize(NewSize);
	}
}

void FGameScene::Resize(std::size_t InAmount)
{
	ObjectIdPool.Resize(InAmount);
	GameObjectAttributes.Resize(InAmount);
}

std::size_t FGameScene::GetGameObjectCount() const
{
	return GameObjectCache.Alive.size();
}

void FGameScene::ActivateGameObject(FGameObject& InGameObject, const bool InActive)
{
	if (InActive)
	{
		GameObjectCache.Activated.push_back(InGameObject);
	}
	else
	{
		GameObjectCache.Deactivated.push_back(InGameObject);
	}
}
