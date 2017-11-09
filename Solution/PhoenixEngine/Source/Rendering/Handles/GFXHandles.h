#ifndef PHOENIX_GFX_HANDLES_H
#define PHOENIX_GFX_HANDLES_H

#include "Rendering/Handles/FontHandles.h"
#include "Rendering/Handles/ImageHandles.h"
#include "Rendering/Handles/ModelHandles.h"
#include "Rendering/Handles/ShaderHandles.h"

namespace Phoenix
{
	class FGFXHandles
	{
	public:
		FFontHandles& GetFontHandles();
		const FFontHandles& GetFontHandles() const;

		FImageHandles& GetImageHandles();
		const FImageHandles& GetImageHandles() const;

		FModelHandles& GetModelHandles();
		const FModelHandles& GetModelHandles() const;

		FShaderHandles& GetShaderHandles();
		const FShaderHandles& GetShaderHandles() const;

		void LogErrorsIfReferencesRemain();

		void ForceClearResources();

	protected:
	private:
		FFontHandles FontHandles;
		FModelHandles ModelHandles;
		FImageHandles ImageHandles;
		FShaderHandles ShaderHandles;
	};
}

#endif
