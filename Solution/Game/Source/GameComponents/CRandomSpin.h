#ifndef PHOENIX_C_RANDOM_SPIN_H
#define PHOENIX_C_RANDOM_SPIN_H

#include "Math/Vector3D.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	struct CRandomSpin
	{
		FVector3D RotationAxis{ NVector3D::Up };
		Float32 RotationSpeed{ 1.57f }; // Radians
	};
}

#endif
