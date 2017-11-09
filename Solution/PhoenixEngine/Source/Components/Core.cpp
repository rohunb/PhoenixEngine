#include "Stdafx.h"
#include "Core.h"

using namespace Phoenix;

BaseCore::BaseCore(const ComponentFilter& Filter) : CompFilter(Filter)
{

}

FGameScene& BaseCore::GetGameScene() const
{
	return *GameScene;
}

std::vector<FGameObject> BaseCore::GetEntities() const
{
	return Entities;
}

const ComponentFilter& BaseCore::GetComponentFilter() const
{
	return CompFilter;
}

void BaseCore::Add(FGameObject& InGameObject)
{
	Entities.push_back(InGameObject);
}

BaseCore::~BaseCore()
{

}

void BaseCore::Remove(FGameObject& InGameObject)
{
	for (auto e = Entities.begin(); e != Entities.end(); ++e)
	{
		if (*e == InGameObject)
		{
			Entities.erase(e);
			return;
		}
	}
}

