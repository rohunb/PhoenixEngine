#include "Stdafx.h"
#include "Rendering/Model/ModelInstance.h"

using namespace Phoenix;

void FModelInstance::SetMaterial(const FMaterial& InMaterial)
{
	Material = InMaterial;
}

void FModelInstance::SetModel(const FString& InModel)
{
	Model = InModel;
}

void FModelInstance::SetModel(FString&& InModel)
{
	Model = std::move(InModel);
}

void FModelInstance::SetOrigin(const FVector3D& InOrigin)
{
	Origin = InOrigin;
}

void FModelInstance::SetPosition(const FVector3D& InPosition)
{
	Position = InPosition;
}

void FModelInstance::SetRotation(const FQuaternion& InRotation)
{
	Rotation = InRotation;
}

void FModelInstance::SetScale(const FVector3D& InScale)
{
	Scale = InScale;
}

const FMaterial& FModelInstance::GetMaterial() const
{
	return Material;
}

const FString& FModelInstance::GetModel() const
{
	return Model;
}

const FVector3D& FModelInstance::GetOrigin() const
{
	return Origin;
}

const FVector3D& FModelInstance::GetPosition() const
{
	return Position;
}

const FQuaternion& FModelInstance::GetRotation() const
{
	return Rotation;
}

const FVector3D& FModelInstance::GetScale() const
{
	return Scale;
}
