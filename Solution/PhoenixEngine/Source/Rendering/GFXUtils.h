#pragma once

#include "Utility/Containers/Array.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/String.h"
#include "Utility/Misc/StringStream.h"
#include "Rendering/GFXTypes.h"

namespace Phoenix
{
	namespace EAssetPath
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			Fonts,
			Models,
			Shaders,
			Textures,
			Count
		};

		const FChar* const Get(const Value AssetPath);
	}

	namespace GFXUtils
	{
		FString GetFontKey(const FChar* const FileName, const FontPixelSizeT PixelSize);

		void LogOpenGLInfo();
	}
}
