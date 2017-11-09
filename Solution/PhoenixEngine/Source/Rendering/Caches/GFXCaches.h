#ifndef PHOENIX_GFX_CACHES_H
#define PHOENIX_GFX_CACHES_H

#include "Rendering/Caches/FontCache.h"
#include "Rendering/Caches/ImageCache.h"
#include "Rendering/Caches/ModelCache.h"
#include "Rendering/Caches/ShaderCache.h"

namespace Phoenix
{
	class FGFXCaches
	{
	public:
		FFontCache& GetFontCache();
		const FFontCache& GetFontCache() const;

		FModelCache& GetModelCache();
		const FModelCache& GetModelCache() const;

		FImageCache& GetImageCache();
		const FImageCache& GetImageCache() const;

		FShaderCache& GetShaderCache();
		const FShaderCache& GetShaderCache() const;

	protected:
	private:
		FFontCache FontCache;
		FModelCache ModelCache;
		FImageCache ImageCache;
		FShaderCache ShaderCache;
	};
}

#endif
