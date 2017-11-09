#include "Stdafx.h"
#include "Utility/Misc/String.h"

#include "Utility/Debug/Assert.h"

using namespace Phoenix;

FString NString::ExtractFileName(const FString& FileDirectory)
{
	const FChar* const PathDelimiters = "/\\";

	const size_t Index = FileDirectory.find_last_of(PathDelimiters);

	if (Index == FString::npos)
	{
		return FileDirectory;
	}

	const FString FileName = FileDirectory.substr(Index + 1);
	return FileName;
}

const FChar* NString::ExtractFileName(const FChar* const FileDirectory)
{
	const SizeT Length = GetLength(FileDirectory);
	if (Length == 0)
	{
		return FileDirectory;
	}

	const FChar* Current = FileDirectory + Length;
	while (Current != FileDirectory)
	{
		if (Current[0] != '/' && Current[0] != '\\')
		{
			--Current;
			continue;
		}

		return Current + 1;
	}

	return Current;
}

const FChar* NString::ExtractFunctionName(const FChar* const FullName)
{
	const SizeT Length = GetLength(FullName);
	if (Length == 0)
	{
		return FullName;
	}

	const FChar* Current = FullName + Length;
	while (Current != FullName)
	{
		if (Current[0] != ':')
		{
			--Current;
			continue;
		}

		return Current + 1;
	}

	return Current;
}

FString NString::ExtractExtension(const FString& File)
{
	const FChar* const ExtensionDelimiter = ".";
	const SizeT Index = File.find_last_of(ExtensionDelimiter);

	if (Index == FString::npos)
	{
		return "";
	}

	const FString Extension = File.substr(Index + 1);
	return Extension;
}

SizeT NString::FindLastOf(
	const FChar* const Str,
	const SizeT StrLength,
	const FChar* const Contents)
{
	const bool StrIsInvalid = Str == nullptr || StrLength == 0 || Str[0] == '\0';
	const bool ContentsAreInvalid = Contents == nullptr || Contents[0] == '\0';

	if (StrIsInvalid || ContentsAreInvalid)
	{
		return FString::npos;
	}

	const SizeT ContentsLength = GetLength(Contents);

	for (SizeT I = StrLength - 1; ; --I)
	{
		for (SizeT J = 0; J < ContentsLength; ++J)
		{
			if (Str[I] == Contents[J])
			{
				return I;
			}
		}

		if (I == 0)
		{
			break;
		}
	}

	return FString::npos;
}

SizeT NString::GetLength(const FChar* const String)
{
	if (!String)
	{
		return 0;
	}

	const SizeT Length = std::strlen(String);
	return Length;
}

bool NString::HasExtension(const FChar* const File, const FChar* const Extension)
{
	const FChar* const ExtensionDelimiter = ".";

	const SizeT Length = GetLength(File);
	const SizeT ExtLength = GetLength(Extension);
	const SizeT Index = FindLastOf(File, Length, ExtensionDelimiter);
	const SizeT FileExtLength = Length - Index - 1;

	if (Length == 0 || ExtLength == 0 || Index == FString::npos || FileExtLength != ExtLength)
	{
		return false;
	}

	for (SizeT I = 0; I < FileExtLength; ++I)
	{
		const SizeT FileExtIndex = I + Index + 1;
		F_Assert(FileExtIndex < Length, "Index out of bounds.");
		F_Assert(I < FileExtLength, "Index out of bounds.");

		const auto FileExtChar = std::tolower(File[FileExtIndex]);
		const auto ExtChar = std::tolower(Extension[I]);

		if (FileExtChar != ExtChar)
		{
			return false;
		}
	}

	return true;
}

void NString::ToLower(FString& String)
{
	for (SizeT I = 0, Max = String.size(); I < Max; ++I)
	{
		String[I] = std::tolower(String[I]);
	}
}

void NString::ToLower(FChar* const String)
{
	if (!String)
	{
		return;
	}

	SizeT Index = 0;
	while (String[Index] != '\0')
	{
		String[Index] = std::tolower(String[Index]);
		++Index;
	}
}
