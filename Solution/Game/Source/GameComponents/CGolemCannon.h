#ifndef PHOENIX_C_GOLEM_CANNON_H
#define PHOENIX_C_GOLEM_CANNON_H

#include "Math/Vector3D.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	struct CGolemCannon
	{
		const FVector3D ShootDirection{ NVector3D::Right };

		const Float32 ShootSpeed{ 2.0f };
	};
}

#endif
