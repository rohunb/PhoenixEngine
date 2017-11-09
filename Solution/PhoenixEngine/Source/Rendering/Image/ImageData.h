#ifndef PHOENIX_IMAGE_DATA_H
#define PHOENIX_IMAGE_DATA_H

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	namespace EPixelFormat
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			Unknown,
			Luminous,
			LuminousAlpha,
			RGB,
			RGBA,
			Count
		};
	}

	namespace ETextureType
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			Diffuse,
			Normal,
			Specular,
			Count
		};
	}

	struct FImageData
	{
		typedef UInt8* PixelsT;
		typedef UInt32 PixelCountT;
		typedef UInt16 WidthT;
		typedef UInt16 HeightT;

		PixelsT Pixels{ nullptr };
		WidthT Width{ 0 };
		HeightT Height{ 0 };
		EPixelFormat::Value PixelFormat{ EPixelFormat::Unknown };
	};
}

#endif
