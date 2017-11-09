#include "Stdafx.h"
#include "Rendering/Text/Font.h"

#include "ExternalLib/FreeTypeIncludes.h"
#include "Utility/Debug/Assert.h"
#include "Rendering/Debug/GFXDebug.h"

using namespace Phoenix;

FFont::FFont(FFont&& RHS)
	: FTFace(RHS.FTFace)
	, Entries(std::move(RHS.Entries))
{
	RHS.PostMoveReset();
}

FFont& FFont::operator=(FFont&& RHS)
{
	if (this == &RHS)
	{
		return *this;
	}

	FTFace = RHS.FTFace;
	Entries = std::move(RHS.Entries);
	RHS.PostMoveReset();

	return *this;
}

FFont::~FFont()
{
	DeInit();
}

void FFont::Init(FT_LibraryRec_& FTLibrary, const FChar* const FilePath)
{
#if PHOENIX_FREE_TYPE_AVAILABLE

	DeInit();

	F_Assert(FilePath, "FilePath is null.");

	const FT_Error FTError = FT_New_Face(&FTLibrary, FilePath, 0, &FTFace);
	F_GFXLogErrorIf(FTError == FT_Err_Unknown_File_Format, "This font format is unsupported.");
	if (FTError != FT_Err_Ok)
	{
		F_GFXLogError("Font failed to initialize. Error: " << FTError);
		DeInit();
		return;
	}

	F_GFXLog("Created font.");
	F_Assert(IsValid(), "Init succeeded but this class is still invalid.");

#endif
}

bool FFont::IsValid() const
{
	const bool Result = FTFace != nullptr;
	return Result;
}

void FFont::DeInit()
{
#if PHOENIX_FREE_TYPE_AVAILABLE
	if (FTFace)
	{
		const FT_Error FTError = FT_Done_Face(FTFace);
		F_GFXLogErrorIf(FTError != FT_Err_Ok, "FT_Done_Face failed. Error: " << FTError);
		FTFace = nullptr;
	}

	Entries.clear();
#endif
}

void FFont::GetCharacters(
	const TVector<UInt32>& Characters,
	const FontPixelSizeT PixelSize,
	TVector<FFontChar>& OutFontCharacters)
{
#if PHOENIX_FREE_TYPE_AVAILABLE
	OutFontCharacters.clear();

	const FT_Error FTError = FT_Set_Pixel_Sizes(FTFace, PixelSize, PixelSize);
	if (FTError != FT_Err_Ok)
	{
		F_GFXLogError("Failed to set pixel size to " << PixelSize << ".  Error: " << FTError);
		return;
	}

	const auto EntriesIter = Entries.find(PixelSize);
	FEntry* EntryPtr = nullptr;

	if (EntriesIter != Entries.end())
	{
		EntryPtr = &EntriesIter->second;
	}
	else
	{
		auto Result = Entries.emplace(PixelSize, FEntry());
		EntryPtr = &Result.first->second;
	}

	const Float32 StartX = -400.f;
	const Float32 ToPixelConversionValue = 1.f / 64.f;

	Float32 X = StartX;
	Float32 Y = 0.f;
	Float32 YOffset = 0.f;

	for (const auto Char : Characters)
	{
		const auto Iter = EntryPtr->Chars.find(Char);
		if (Iter != EntryPtr->Chars.end())
		{
			const FFontChar& FontChar = Iter->second;
			OutFontCharacters.push_back(FontChar);
			continue;
		}

		if (Char == '\n')
		{
			X = StartX;
			Y -= FTFace->height * ToPixelConversionValue;
			continue;
		}

		if (FT_Load_Char(FTFace, Char, FT_LOAD_RENDER) != FT_Err_Ok)
		{
			F_GFXLogError("FT_Load_Char failed for char: " << Char);
			continue;
		}

		FT_GlyphSlot& FTGlyph = FTFace->glyph;
		const FT_Bitmap& FTBitmap = FTGlyph->bitmap;

		if (!FTBitmap.buffer)
		{
			X += FTGlyph->advance.x * ToPixelConversionValue;
			continue;
		}

		// Size: Width/Rows
		// Bearing: BitmapLeft / BitmapTop

		const Float32 XPos = X + static_cast<Float32>(FTGlyph->bitmap_left);
		const Float32 YPos = Y - static_cast<Float32>(FTBitmap.rows - FTGlyph->bitmap_top);

		const Float32 Width = static_cast<Float32>(FTBitmap.width);
		const Float32 Height = static_cast<Float32>(FTBitmap.rows);

		const FVector2D BottomLeft(XPos, YPos);
		const FVector2D BottomRight(XPos + Width, YPos);
		const FVector2D TopRight(XPos + Width, YPos + Height);
		const FVector2D TopLeft(XPos, YPos + Height);

		F_GFXLogTrace("Generating data for " << (UInt8)Char << ". @ { " << X << ", " << Y << " }");

		//EntryPtr->Chars.emplace(FontCharValue, FontChar);
		//OutFontCharacters.push_back(FontChar);

		X += FTGlyph->advance.x * ToPixelConversionValue;
	}

#endif
}

void FFont::PostMoveReset()
{
	FTFace = nullptr;
	F_Assert(Entries.empty(), "Entries should be empty after they've been moved to another object.");
}
