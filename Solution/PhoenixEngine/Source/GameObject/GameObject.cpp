#include "Stdafx.h"
#include "GameObject/GameObject.h"
#include "Core/GameScene.h"

using namespace Phoenix;

FGameObject::FGameObject()
{
}

FGameObject::FGameObject(FGameScene& inGameScene, ID inId) :
	Id(inId),
	GameScene(&inGameScene)
{
}

FGameObject::~FGameObject()
{
}

void FGameObject::AddComponent(BaseComponent* inComponent, TypeId inComponentTypeId)
{
	GameScene->GameObjectAttributes.Storage.AddComponent(this, inComponent, inComponentTypeId);
}

const FGameObject::ID& FGameObject::GetId() const
{
	return Id;
}

BaseComponent& FGameObject::GetComponent(TypeId InTypeId) const
{
	return GameScene->GameObjectAttributes.Storage.GetComponent(*this, InTypeId);
}

void FGameObject::SetActive(const bool InActive)
{
	GameScene->ActivateGameObject(*this, InActive);
}

bool FGameObject::operator==(const FGameObject& GameObject) const
{
	return Id == GameObject.Id && GameObject.GameScene == GameScene;
}

void FGameObject::RemoveComponent(TypeId InComponentTypeId)
{
	GameScene->GameObjectAttributes.Storage.RemoveComponent(*this, InComponentTypeId);
}
