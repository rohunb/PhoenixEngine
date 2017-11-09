#include "Stdafx.h"
#include "Utility/Misc/ConsoleWindow.h"

#include "ExternalLib/Win32Includes.h"
#include "Utility/Misc/Primitives.h"

using namespace Phoenix;

void FConsoleWindow::Hide()
{
#if _WIN32
	/* Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms683175%28v=vs.85%29.aspx */
	HWND ConsoleWindow = GetConsoleWindow();
	const Int32 ShowWindowCommand = SW_HIDE;

	/* Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633548%28v=vs.85%29.aspx */
	ShowWindow(ConsoleWindow, ShowWindowCommand);
#endif
}

void FConsoleWindow::Show()
{
#if _WIN32
	/* Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms683175%28v=vs.85%29.aspx */
	HWND ConsoleWindow = GetConsoleWindow();
	const Int32 ShowWindowCommand = SW_SHOW;

	/* Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633548%28v=vs.85%29.aspx */
	ShowWindow(ConsoleWindow, ShowWindowCommand);
#endif
}