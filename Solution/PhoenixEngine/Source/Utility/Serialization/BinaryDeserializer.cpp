#include "Stdafx.h"
#include "Utility/Serialization/BinaryDeserializer.h"

using namespace Phoenix;

FBinaryDeserializer::FBinaryDeserializer(FInputFileStream& InputStream)
	: ISerializer<FBinaryDeserializer>(this)
	, InputStream(InputStream)
{}

void FBinaryDeserializer::Load(TVector<bool>& VectorData)
{
	SizeT VectorSize;
	Serialize(VectorSize);

	VectorData.resize(VectorSize);

	for (auto&& Element : VectorData)
	{
		bool BoolValue;
		Serialize(BoolValue);
		Element = BoolValue;
	}
}