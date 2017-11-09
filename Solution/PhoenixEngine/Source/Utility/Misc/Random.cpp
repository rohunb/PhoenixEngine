#include "Stdafx.h"
#include "Utility/Misc/Random.h"

using namespace Phoenix;

bool FRandom::Bool()
{
	return Bool(0.5);
}

bool FRandom::Bool(Float64 TrueProbability)
{
	std::bernoulli_distribution Distribution { TrueProbability };
	return Distribution(RandomEngine);
}

FString FRandom::String(UInt32 Size)
{
	FString RandomString;
	RandomString.reserve(Size);

	for (UInt32 I = 0; I < Size; ++I)
	{
		FChar Char = Range(TNumericLimits<FChar>::min(), TNumericLimits<FChar>::max());
		RandomString.push_back(Char);
	}

	return RandomString;
}

FVector3D FRandom::UnitVector3()
{
	const Float32 X = Range(-1.0f, 1.0f);
	const Float32 Y = Range(-1.0f, 1.0f);
	const Float32 Z = Range(-1.0f, 1.0f);

	return FVector3D(X, Y, Z);
}
