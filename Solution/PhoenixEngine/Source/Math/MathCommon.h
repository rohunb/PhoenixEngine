#ifndef PHOENIX_MATH_COMMON_H
#define PHOENIX_MATH_COMMON_H

#include "ExternalLib/GLMIncludes.h"
#include "Utility/Misc/Primitives.h"
#include "Math/Matrix4D.h"
#include "Math/Quaternion.h"
#include "Math/Vector3D.h"

namespace Phoenix
{
	namespace NMatrix4D
	{
		static FMatrix4D Create(const FVector3D& Position, const FVector3D& Scale, const FQuaternion& Rotation)
		{
			const FVector3D AxisInitial = glm::axis(Rotation);
			const Float32 Angle = glm::angle(Rotation);

			const Float32 Cos = glm::cos(Angle);
			const Float32 Sin = glm::sin(Angle);

			const FVector3D Axis(glm::normalize(AxisInitial));
			const FVector3D Temp((1.f - Cos) * Axis);

			FMatrix4D Matrix(glm::uninitialize);
			Matrix[0][0] = (Cos + Temp[0] * Axis[0]) * Scale.x;
			Matrix[0][1] = 0 + Temp[0] * Axis[1] + Sin * Axis[2];
			Matrix[0][2] = 0 + Temp[0] * Axis[2] - Sin * Axis[1];
			Matrix[0][3] = 0;

			Matrix[1][0] = 0 + Temp[1] * Axis[0] - Sin * Axis[2];
			Matrix[1][1] = (Cos + Temp[1] * Axis[1]) * Scale.y;
			Matrix[1][2] = 0 + Temp[1] * Axis[2] + Sin * Axis[0];
			Matrix[1][3] = 0;

			Matrix[2][0] = 0 + Temp[2] * Axis[0] + Sin * Axis[1];
			Matrix[2][1] = 0 + Temp[2] * Axis[1] - Sin * Axis[0];
			Matrix[2][2] = (Cos + Temp[2] * Axis[2]) * Scale.z;
			Matrix[2][3] = 0;

			Matrix[3] = FMatrix4D::col_type(Position, 1.f);

			return Matrix;
		}
	}

	namespace NVector3D
	{
		static Float32 GetLengthSq(const FVector3D& Vertex)
		{
			const Float32 LengthSq = Vertex.x * Vertex.x + Vertex.y * Vertex.y + Vertex.z * Vertex.z;
			return LengthSq;
		}
	}
}

#endif
