#ifndef PHOENIX_GFX_ENGINE_H
#define PHOENIX_GFX_ENGINE_H

#include "Utility/Handle/Handle.h"
#include "Utility/Misc/Memory.h"
#include "Rendering/Image/Image.h"
#include "Rendering/Model/Model.h"
#include "Rendering/Text/Font.h"

namespace Phoenix
{
	class FGFXEngine
	{
		friend struct FGFXEngineInternals;
	public:
		struct FInitParams
		{
			TRawPtr<class IWindow> Window;
		};

		FGFXEngine();

		FGFXEngine(const FGFXEngine&) = delete;
		FGFXEngine& operator=(const FGFXEngine&) = delete;

		FGFXEngine(FGFXEngine&&) = delete;
		FGFXEngine& operator=(FGFXEngine&&) = delete;

		~FGFXEngine();

		void Init(const FInitParams& InitParams);

		bool IsValid() const;

		void ForceShutDown();

		bool IsShutDown() const;

		void SetUpNextRender();

		class FGFXScene& GetScene();
		const class FGFXScene& GetScene() const;
		
	protected:
	private:
		typedef void(FGFXEngine::*DrawFunction)();

		TUniquePtr<struct FGFXEngineInternals> PImplData;

		void ThreadRun();

		void ThreadInit();

		void ThreadDeInit();

		void ThreadDraw();
		
		THandle<FFont> ThreadLoadAndCacheFont(const FChar* const FontFileName);

		THandle<FImage> ThreadLoadAndCacheImage(const FChar* const ImageFileName);

		THandle<FModel> ThreadLoadAndCacheModel(const FChar* const ModelFileName);

		class FGFXHandles& GetHandles();
		const class FGFXHandles& GetHandles() const;

		class FGFXCaches& GetCaches();
		const class FGFXCaches& GetCaches() const;

		struct FGFXEngineInternals& Get();

		const struct FGFXEngineInternals& Get() const;

		void ThreadDrawInternal();

		void ThreadEmptyFunction();

		bool ThreadInitShaders();

		void ThreadDrawScene();
	};
}

#endif
