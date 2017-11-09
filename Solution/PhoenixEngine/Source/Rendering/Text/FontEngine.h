#ifndef PHOENIX_FONT_ENGINE_H
#define PHOENIX_FONT_ENGINE_H

#include "Utility/Handle/Handles.h"
#include "Utility/Misc/Primitives.h"
#include "Rendering/GFXEngine.h"
#include "Rendering/GFXTypes.h"
#include "Rendering/GL/GLInterface.h"
#include "Rendering/Text/Font.h"

struct FT_LibraryRec_;

namespace Phoenix
{
	class FFontEngine
	{
	public:
		FFontEngine() = default;

		FFontEngine(const FFontEngine&) = delete;
		FFontEngine& operator=(const FFontEngine&) = delete;

		FFontEngine(FFontEngine&&);
		FFontEngine& operator=(FFontEngine&&);

		~FFontEngine();

		void Init(const TRawPtr<class FGFXEngine> InGFXEngine);

		bool IsValid() const;

		void DeInit();

		THandle<FFont> LoadAndCacheFont(const FString& FontFileName);

	private:
		TRawPtr<class FGFXEngine> GFXEngine;
		FT_LibraryRec_* FTLibrary{ nullptr };
				
		void PostMoveReset();
	};
}

#endif
