#ifndef PHOENIX_CONSOLE_COLOR_H
#define PHOENIX_CONSOLE_COLOR_H

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	namespace EConsoleColor
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			White,
			Grey,
			Red,
			Green,
			Blue,
			Yellow,
			Pink,
			LightBlue,
			Count,
		};
	}

	struct FConsoleColor
	{
	public:
		explicit FConsoleColor(const EConsoleColor::Value InColor);

		bool IsValid() const;

		static void Set(const EConsoleColor::Value Color);

		static void Set(const FConsoleColor Color);

	protected:
	private:
		EConsoleColor::Value Color{ EConsoleColor::White };
	};
}

#endif
