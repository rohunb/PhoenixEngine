#include "Stdafx.h"
#include "Transform.h"
#include "GameObject/GameObject.h"

using namespace Phoenix;

void Transform::Init()
{
}

Transform::~Transform()
{
}

void Transform::SetPosition(const FVector3D& InPosition)
{
	Position = InPosition;
}
