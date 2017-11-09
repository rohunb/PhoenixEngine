#ifndef PHOENIX_SHADER_CACHE_H
#define PHOENIX_SHADER_CACHE_H

#include "Utility/Containers/NamedItemCache.h"
#include "Utility/Handle/Handle.h"
#include "Rendering/Shader/Shader.h"

namespace Phoenix
{
	class FShaderCache : public TNamedItemCache<THandle<FShader>> {};
}

#endif
