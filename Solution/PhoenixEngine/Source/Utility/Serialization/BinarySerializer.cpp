#include "Stdafx.h"
#include "Utility/Serialization/BinarySerializer.h"

using namespace Phoenix;

FBinarySerializer::FBinarySerializer(FOutputFileStream& OutputStream)
	: ISerializer<FBinarySerializer>(this)
	, OutputStream(OutputStream)
{}

void FBinarySerializer::Save(TVector<bool>& VectorData)
{
	const SizeT VectorSize = VectorData.size();
	Serialize(VectorSize);

	for (auto&& Element: VectorData)
	{
		Serialize(static_cast<bool>(Element));
	}
}