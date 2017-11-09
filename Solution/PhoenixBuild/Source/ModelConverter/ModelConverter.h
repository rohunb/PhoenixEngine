#pragma once

#include "Utility/Misc/Primitives.h"
#include "Math/Vector3D.h"

namespace Phoenix
{
	class FModelConverter
	{
	public:
		static void ConvertModelToBinary(
			const FChar* const InputPath, 
			const FChar* const OutputPath,
			const FVector3D& OriginOffset = FVector3D(),
			const Float32 NormalizedSize = 0.f);
	};
}
