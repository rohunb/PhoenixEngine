#ifndef PHOENIX_C_TRANSFORM_H
#define PHOENIX_C_TRANSFORM_H

#include "Math/Vector3D.h"

namespace Phoenix
{
	struct CTransform
	{
		FVector3D Position;
		FVector3D Scale { 1.0f };
		FQuaternion Rotation;

		CTransform() = default;

		explicit CTransform(const FVector3D& Position)
			: Position(Position)
		{}

		CTransform(const FVector3D& Position, const FQuaternion& Rotation)
			: Position(Position)
			, Rotation(Rotation)
		{}

		CTransform(const FVector3D& Position, const FVector3D& Scale)
			: Position(Position)
			, Scale(Scale)
		{}

		CTransform(const FVector3D& Position, const FVector3D& Scale, const FQuaternion& Rotation)
			: Position(Position)
			, Scale(Scale)
			, Rotation(Rotation)
		{}
	};
}

#endif
