#include "Stdafx.h"
#include "Rendering/Camera.h"

#include "Utility/Debug/Assert.h"
#include "Math/Math.h"
#include "Math/MathCommon.h"
#include "Math/MatrixTransform.h"

using namespace Phoenix;

const FVector3D FCamera::DefaultDirection = FVector3D(0, 0, -1);
const FVector3D FCamera::DefaultUpVector = FVector3D(0, 1, 0);

void FCamera::SetPerspective(
	const Float32 FieldOfView,
	const Float32 AspectRatio,
	const Float32 Near,
	const Float32 Far)
{
	F_Assert(FieldOfView > 0.f, "Field of view must be greater than 0.");
	F_Assert(AspectRatio > 0.f, "Aspect ratio must be greater than 0..");
	F_Assert(Near != Far, "Near and far must not be equal.");

	Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
}

void FCamera::SetOrthographic(
	const Float32 Left,
	const Float32 Right,
	const Float32 Bottom,
	const Float32 Top)
{
	F_Assert(Left != Right, "Left must not be equal to right.");
	F_Assert(Bottom != Top, "Bottom must not be equal to top.");

	Projection = glm::ortho(Left, Right, Bottom, Top);
}

void FCamera::SetView(
	const FVector3D& InPosition,
	const FVector3D& InLookAtTarget,
	const FVector3D& InUpVector)
{
	SetPosition(InPosition);
	SetLookAtTarget(InLookAtTarget);
	SetUpVector(InUpVector);
	UpdateViewMatrix();
}

void FCamera::SetPosition(const FVector3D& InPosition)
{
	Position = InPosition;
}

void FCamera::SetDirection(const FVector3D& InDirection)
{
	F_Assert(FMathf::IsCloseTo(glm::length(InDirection), 1.f), "Direction is not normalized.");
	Direction = InDirection;
}

void FCamera::SetLookAtTarget(const FVector3D& Target)
{
	F_Assert(Position != Target, "The camera's target must not be equal to the camera's position.");
	Direction = glm::normalize(Target - Position);
}

void FCamera::SetUpVector(const FVector3D& InUpVector)
{
	F_Assert(FMathf::IsCloseTo(glm::length(InUpVector), 1.f), "UpVector is not normalized.");
	UpVector = InUpVector;
}

void FCamera::UpdateViewMatrix()
{
	F_Assert(FMathf::IsCloseTo(glm::length(Direction), 1.f), "Direction is not normalized.");
	F_Assert(FMathf::IsCloseTo(glm::length(UpVector), 1.f), "UpVector is not normalized.");
	View = glm::lookAt(Position, Position + Direction, UpVector);
}

const FMatrix4D& FCamera::GetProjection() const
{
	return Projection;
}

const FMatrix4D& FCamera::GetView() const
{
	return View;
}

const FVector3D& FCamera::GetPosition() const
{
	return Position;
}

const FVector3D& FCamera::GetDirection() const
{
	return Direction;
}

const FVector3D& FCamera::GetUpVector() const
{
	return UpVector;
}

const Float32* const FCamera::GetProjectionPtr() const
{
	const Float32* const ProjectionPtr = glm::value_ptr(Projection);
	return ProjectionPtr;
}

const Float32* const FCamera::GetViewPtr() const
{
	const Float32* const ViewPtr = glm::value_ptr(View);
	return ViewPtr;
}
