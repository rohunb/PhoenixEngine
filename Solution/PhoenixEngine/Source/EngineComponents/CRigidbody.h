#ifndef PHOENIX_C_RIGIDBODY_H
#define PHOENIX_C_RIGIDBODY_H

#include "Math/Vector3D.h"

namespace Phoenix
{
	struct CRigidbody
	{
		FVector3D Velocity;
		FVector3D Acceleration;

		CRigidbody() = default;

		explicit CRigidbody(const FVector3D& Velocity)
			: Velocity(Velocity)
		{}

		CRigidbody(const FVector3D& Velocity, const FVector3D& Acceleration)
			: Velocity(Velocity)
			, Acceleration(Acceleration)
		{}
	};
}

#endif
