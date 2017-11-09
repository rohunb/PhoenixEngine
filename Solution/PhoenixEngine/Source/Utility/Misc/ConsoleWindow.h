#ifndef PHOENIX_CONSOLE_WINDOW_H
#define PHOENIX_CONSOLE_WINDOW_H

namespace Phoenix
{
	struct FConsoleWindow
	{
		FConsoleWindow() = delete;

		static void Hide();

		static void Show();
	};
}

#endif
