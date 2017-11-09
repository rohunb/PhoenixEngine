#include "Stdafx.h"
#include "Components/Cores/RenderCore.h"
#include "Components/Transform.h"
#include "Components/ComponentFilter.h"

using namespace Phoenix;

FRenderCore::FRenderCore() : Base(ComponentFilter().Requires<Transform>())
{
}

void FRenderCore::Init()
{

}

void Phoenix::FRenderCore::DeInit()
{

}

void FRenderCore::Update(const FUpdateEvent& UpdateEvent)
{
}

FRenderCore::~FRenderCore()
{
}

void FRenderCore::Render()
{
	auto Renderables = GetEntities();
	for (auto& InEntity : Renderables)
	{
		auto& TransformComponent = InEntity.GetComponent<Transform>();
		TransformComponent.SetPosition(FVector3D(0, 1, 2));
	}
}
