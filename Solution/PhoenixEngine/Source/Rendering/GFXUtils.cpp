#include "Stdafx.h"
#include "Rendering/GFXUtils.h"

#include "Config/GFXCompileConfig.h"
#include "Rendering/Debug/GFXDebug.h"
#include "Rendering/GL/GLInterface.h"
#include "Rendering/GL/GLTypes.h"

#ifndef PHOENIX_GFX_COMPILE_CONFIG
#	error("Config/GFXCompileConfig.h should be included in this file.")
#endif

using namespace Phoenix;
using namespace Phoenix::GL;

namespace Phoenix
{
	namespace EAssetPath
	{
		const FChar* const Get(const Value AssetPath)
		{
			static_assert(EAssetPath::Count == 4, "This table needs updating.");
			TArray<const FChar* const, EAssetPath::Count> LookUpTable =
			{
				"../Assets/Fonts/",
				"../Assets/Models/",
				"../Assets/Shaders/",
				"../Assets/Textures/"
			};

			const FChar* const Result = LookUpTable[AssetPath];
			return Result;
		}
	}

	namespace GFXUtils
	{
		FString GetFontKey(const FChar* const FileName, const FontPixelSizeT PixelSize)
		{
			FStringStream SS;
			SS << FileName << PixelSize;
			const FString FontKey = SS.str();
			return FontKey;
		}

		void LogOpenGLInfo()
		{
#if PHOENIX_GFX_DISPLAY_OPEN_GL_INFO
			F_GLDisplayErrors();
			const SizeT ArraySize = 4;
			const TArray<const GLubyte*, ArraySize> GLInfoArray =
			{
				GL::GetString(EGLInfo::Vendor),
				GL::GetString(EGLInfo::Renderer),
				GL::GetString(EGLInfo::Version),
				GL::GetString(EGLInfo::ShadingLanguageVersion)
			};

			F_GLDisplayErrors();
			const TArray<const FChar* const, ArraySize> GLInfoDescArray =
			{
				"Vendor: ",
				"Renderer: ",
				"Running Version: ",
				"Shading Language Version: "
			};

			F_GFXLog("OpenGL: Compiled For Version 3.3.0");
			for (SizeT I = 0; I < ArraySize; ++I)
			{
				if (GLInfoArray[I])
				{
					F_GFXLog(GLInfoDescArray[I] << GLInfoArray[I]);
				}
			}

#	if PHOENIX_GFX_DISPLAY_OPEN_GL_EXTENSIONS
			const GLubyte* const GLExtensions = GL::GetString(EGLInfo::Extensions);
			const GLubyte* const NoGLExtensions = reinterpret_cast<const GLubyte*>("None.");
			F_GFXLog("OpenGL Extensions: " << (GLExtensions ? GLExtensions : NoGLExtensions));
			F_GLIgnoreErrors();
#	endif
#endif
		}
	}
}
