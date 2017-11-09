#include "ModelConverter/ModelConverter.h"
#include "Rendering/Model/ModelProcessor.h"

using namespace Phoenix;

void FModelConverter::ConvertModelToBinary(
	const FChar* const InputPath, 
	const FChar* const OutputPath,
	const FVector3D& OriginOffset,
	const Float32 NormalizedSize)
{
	FModelProcessor::FLoadParams LoadParams;
	LoadParams.File = InputPath;
	LoadParams.MeshAttributeHints = EMeshAttribute::All;
	LoadParams.OriginOffset = OriginOffset;
	LoadParams.NormalizedSize = NormalizedSize;

	FModelProcessor::FSaveParams SaveParams;
	SaveParams.File = OutputPath;

	FModelProcessor ModelProcessor;
	
	ModelProcessor.Load(LoadParams);
	ModelProcessor.Save(SaveParams);
}
