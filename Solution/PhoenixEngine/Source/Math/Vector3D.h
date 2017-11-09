#ifndef PHOENIX_VECTOR_3D_H
#define PHOENIX_VECTOR_3D_H

#include "ExternalLib/GLMIncludes.h"

namespace Phoenix
{
	typedef glm::vec3 FVector3D;

	namespace NVector3D
	{
		static const FVector3D Up { 0.0f, 1.0f, 0.0f };
		static const FVector3D Down { 0.0f, -1.0f, 0.0f };
		static const FVector3D Left { -1.0f, 0.0f, 0.0f };
		static const FVector3D Right { 1.0f, 0.0f, 0.0f };
		static const FVector3D Forward { 0.0f, 0.0f, -1.0f };
		static const FVector3D Back { 0.0f, 0.0f, 1.0f };
	}
}

#endif
