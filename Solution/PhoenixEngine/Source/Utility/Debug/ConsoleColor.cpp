#include "Stdafx.h"
#include "Utility/Debug/ConsoleColor.h"

#include "ExternalLib/Win32Includes.h"
#include "Utility/Containers/Array.h"

using namespace Phoenix;

FConsoleColor::FConsoleColor(const EConsoleColor::Value InColor)
	: Color(InColor)
{
}

bool FConsoleColor::IsValid() const
{
	const bool bIsValid = Color < EConsoleColor::Count;
	return bIsValid;
}

void FConsoleColor::Set(const EConsoleColor::Value Color)
{
	Set(FConsoleColor(Color));
}

void FConsoleColor::Set(const FConsoleColor Color)
{
#if _WIN32
	static_assert(EConsoleColor::Count == 8, "This table requires updating.");
	static const TArray<WORD, EConsoleColor::Count> LookUpTable =
	{
		/* White		*/FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		/* Grey			*/0,
		/* Red			*/FOREGROUND_RED,
		/* Green		*/FOREGROUND_GREEN,

		/* Blue			*/FOREGROUND_BLUE,
		/* Yellow		*/FOREGROUND_RED | FOREGROUND_GREEN,
		/* Pink			*/FOREGROUND_RED | FOREGROUND_BLUE,
		/* LightBlue	*/FOREGROUND_GREEN | FOREGROUND_BLUE,
	};

	/* Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms683231%28v=vs.85%29.aspx */
	HANDLE const Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	const WORD Attributes = LookUpTable[Color.Color] | FOREGROUND_INTENSITY;

	/* Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms686047%28v=vs.85%29.aspx */
	SetConsoleTextAttribute(Handle, Attributes);
#endif
}