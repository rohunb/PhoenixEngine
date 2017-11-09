#include "Stdafx.h"
#include "GameObject/GameObjectIdPool.h"

using namespace Phoenix;

GameObjectIdPool::GameObjectIdPool(std::size_t InPoolSize) :
	DefaultPoolSize(InPoolSize),
	NextId(0),
	Entities(InPoolSize)
{

}

FGameObject::ID GameObjectIdPool::Create()
{
	FGameObject::ID Id;

	if (!FreeList.empty())
	{
		Id = FreeList.back();
		FreeList.pop_back();
	}
	else
	{
		Id.Index = NextId++;
		Entities[Id.Index] = Id.Counter = 1;
	}
	return Id;
}

std::size_t GameObjectIdPool::GetSize() const
{
	return Entities.size();
}

void GameObjectIdPool::Resize(std::size_t InAmount)
{
	Entities.resize(InAmount);
}

void GameObjectIdPool::Remove(FGameObject::ID InGameObjectId)
{
	auto& Counter = Entities[InGameObjectId.Index];
	++Counter;
	FreeList.emplace_back(InGameObjectId.Index, Counter);
}
