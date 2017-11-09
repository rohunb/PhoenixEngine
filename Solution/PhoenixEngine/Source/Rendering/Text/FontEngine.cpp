#include "Stdafx.h"
#include "Rendering/Text/FontEngine.h"

#include "ExternalLib/FreeTypeIncludes.h"
#include "Utility/Debug/Assert.h"
#include "Rendering/GFXEngine.h"
#include "Rendering/GFXUtils.h"
#include "Rendering/Caches/GFXCaches.h"
#include "Rendering/Debug/GFXDebug.h"
#include "Rendering/Debug/GFXProfiler.h"
#include "Rendering/Handles/GFXHandles.h"
#include "Rendering/Text/Font.h"

using namespace Phoenix;

FFontEngine::FFontEngine(FFontEngine&& RHS)
	: GFXEngine(RHS.GFXEngine)
	, FTLibrary(RHS.FTLibrary)
{
	RHS.PostMoveReset();
}

FFontEngine& FFontEngine::operator=(FFontEngine&& RHS)
{
	if (this == &RHS)
	{
		return *this;
	}

	GFXEngine = RHS.GFXEngine;
	FTLibrary = RHS.FTLibrary;
	RHS.PostMoveReset();

	return *this;
}

FFontEngine::~FFontEngine()
{
	DeInit();
}

void FFontEngine::Init(const TRawPtr<FGFXEngine> InGFXEngine)
{
#if PHOENIX_FREE_TYPE_AVAILABLE
	DeInit();

	F_Assert(InGFXEngine.IsValid(), "InGFXEngine pointer is invalid.");
	GFXEngine = InGFXEngine;

	const FT_Error FTError = FT_Init_FreeType(&FTLibrary);
	if (FTError != FT_Err_Ok)
	{
		F_GFXLogError("Failed to initialize FreeType. Error: " << FTError);
		DeInit();
		return;
	}

	F_Assert(IsValid(), "Init succeeded but this class is invalid.");
#endif
}

bool FFontEngine::IsValid() const
{
	const bool Result = FTLibrary != nullptr;
	return Result;
}

void FFontEngine::DeInit()
{
#if PHOENIX_FREE_TYPE_AVAILABLE
	if (FTLibrary)
	{
		const FT_Error FTError = FT_Done_FreeType(FTLibrary);
		F_GFXLogErrorIf(FTError != FT_Err_Ok, "FT_Done_FreeType failed. Error: " << FTError);
		FTLibrary = nullptr;
	}

	GFXEngine = nullptr;
#endif
}

THandle<FFont> FFontEngine::LoadAndCacheFont(const FString& FontFileName)
{
#if PHOENIX_FREE_TYPE_AVAILABLE

	F_GFXProfile();
	F_Assert(!FontFileName.empty(), "FontFileName is empty.");

	FGFXCaches& Caches = GFXEngine->GetCaches();
	FFontCache& FontCache = Caches.GetFontCache();

	THandle<FFont> Font = FontCache.GetItemCopy(FontFileName);
	if (Font.IsValid())
	{
		return Font;
	}
	
	const FString FullPath = EAssetPath::Get(EAssetPath::Fonts) + FontFileName;

	FGFXHandles& Handles = GFXEngine->GetHandles();
	FFontHandles& FontHandles = Handles.GetFontHandles();
		
	Font = FontHandles.CreateHandle();
	Font->Init(*FTLibrary, FullPath.c_str());

	if (!Font->IsValid())
	{
		F_GFXLogError("Failed to initialize font.");
		Font.DeInit();
		return Font;
	}

	FontCache.AddEntry(FontFileName, Font);
	return Font;

#else
	return THandle<FFont>();
#endif
}

void FFontEngine::PostMoveReset()
{
	GFXEngine = nullptr;
	FTLibrary = nullptr;
}
