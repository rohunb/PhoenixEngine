#include "Stdafx.h"
#include "Rendering/Caches/GFXCaches.h"

using namespace Phoenix;

FFontCache& FGFXCaches::GetFontCache()
{
	return FontCache;
}

const FFontCache& FGFXCaches::GetFontCache() const
{
	return FontCache;
}

FModelCache& FGFXCaches::GetModelCache()
{
	return ModelCache;
}

const FModelCache& FGFXCaches::GetModelCache() const
{
	return ModelCache;
}

FImageCache& FGFXCaches::GetImageCache()
{
	return ImageCache;
}

const FImageCache& FGFXCaches::GetImageCache() const
{
	return ImageCache;
}

FShaderCache& FGFXCaches::GetShaderCache()
{
	return ShaderCache;
}

const FShaderCache& FGFXCaches::GetShaderCache() const
{
	return ShaderCache;
}
