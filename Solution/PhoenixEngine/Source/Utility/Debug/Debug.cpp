#include "Stdafx.h"
#include "Utility/Debug/Debug.h"

#include "ExternalLib/Win32Includes.h"

using namespace Phoenix;

void FDebug::Break()
{
#if _WIN32
	/* Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms679297%28v=vs.85%29.aspx */
	DebugBreak();
#else
#	pragma message("Debug.cpp - FDebug::Break() is missing its implementation on this platform.")
#endif
}