#include "Stdafx.h"
#include "Rendering/Handles/GFXHandles.h"

#include "Rendering/Debug/GFXDebug.h"

using namespace Phoenix;

FFontHandles& FGFXHandles::GetFontHandles()
{
	return FontHandles;
}

const FFontHandles& FGFXHandles::GetFontHandles() const
{
	return FontHandles;
}

FImageHandles& FGFXHandles::GetImageHandles()
{
	return ImageHandles;
}

const FImageHandles& FGFXHandles::GetImageHandles() const
{
	return ImageHandles;
}

FModelHandles& FGFXHandles::GetModelHandles()
{
	return ModelHandles;
}

const FModelHandles& FGFXHandles::GetModelHandles() const
{
	return ModelHandles;
}

FShaderHandles& FGFXHandles::GetShaderHandles()
{
	return ShaderHandles;
}

const FShaderHandles& FGFXHandles::GetShaderHandles() const
{
	return ShaderHandles;
}

void FGFXHandles::LogErrorsIfReferencesRemain()
{
	F_GFXLogErrorIf(FontHandles.GetDataSize(), FontHandles.GetDataSize() << " font handles remain.");
	F_GFXLogErrorIf(ModelHandles.GetDataSize(), ModelHandles.GetDataSize() << " model handles remain.");
	F_GFXLogErrorIf(ImageHandles.GetDataSize(), ImageHandles.GetDataSize() << " image handles remain.");
	F_GFXLogErrorIf(ShaderHandles.GetDataSize(), ShaderHandles.GetDataSize() << " shader handles remain.");
}

void FGFXHandles::ForceClearResources()
{
	for (SizeT I = 0; I < FontHandles.GetDataSize(); ++I)
	{
		FontHandles[I].DeInit();
	}

	for (SizeT I = 0; I < ModelHandles.GetDataSize(); ++I)
	{
		ModelHandles[I].DeInit();
	}

	for (SizeT I = 0; I < ImageHandles.GetDataSize(); ++I)
	{
		ImageHandles[I].DeInit();
	}

	for (SizeT I = 0; I < ShaderHandles.GetDataSize(); ++I)
	{
		ShaderHandles[I].DeInit();
	}
}
