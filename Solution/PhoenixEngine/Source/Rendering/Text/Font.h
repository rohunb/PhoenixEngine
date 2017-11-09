#ifndef PHOENIX_FONT_H
#define PHOENIX_FONT_H

#include "Utility/Containers/UnorderedMap.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Misc/Memory.h"
#include "Utility/Misc/Primitives.h"
#include "Math/Rectangle2D.h"
#include "Math/Vector2D.h"
#include "Rendering/GFXTypes.h"
#include "Rendering/GL/GLInterface.h"
#include "Rendering/Image/Image.h"

struct FT_FaceRec_;
struct FT_LibraryRec_;

namespace Phoenix
{
	namespace ETextAlignment
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			TopLeft,
			TopCenter,
			TopRight,
			MidLeft,
			MidCenter,
			MidRight,
			BottomLeft,
			BottomCenter,
			BottomRight
		};
	}
	
	namespace ETextOverflow
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			None,
			WrapX,
			WrapXTruncateY,
			TruncateXY,
		};
	}

	namespace ETextSize
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			None,
			ShrinkToFit
		};
	}

	namespace ETextStyle
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			Normal,
			Bold,
			Italics,
			BoldItalics
		};
	}

	struct FFontChar
	{
		FVector2D PositionMin;
		FVector2D PositionDim;
		
		FVector2D TexCoordMin;
		FVector2D TexCoordDim;

		GL::ImageIDT Image{ 0 };
	};

	class FFont
	{
	public:
		FFont() = default;

		FFont(const FFont&) = delete;
		FFont& operator=(const FFont&) = delete;

		FFont(FFont&&);
		FFont& operator=(FFont&&);

		~FFont();

		void Init(FT_LibraryRec_& FTLibrary, const FChar* const FilePath);

		bool IsValid() const;

		void DeInit();

		void GetCharacters(
			const TVector<UInt32>& Characters, 
			const FontPixelSizeT PixelSize, 
			TVector<FFontChar>& OutFontCharacters);

	private:
		struct FEntry
		{
			TVector<FImage> Atlases;
			TUnorderedMap<UInt64, FFontChar> Chars;
		};

		FT_FaceRec_* FTFace{ nullptr };
		TUnorderedMap<FontPixelSizeT, FEntry> Entries;

		void PostMoveReset();
	};
}

#endif
