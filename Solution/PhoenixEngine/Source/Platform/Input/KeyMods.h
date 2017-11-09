#ifndef PHOENIX_KEY_MODS_H
#define PHOENIX_KEY_MODS_H

#include "Utility/Containers/Array.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	/*!
	 * \brief Flags for modifiers that were active during a FKeyEvent.
	 */
	namespace EKeyMods
	{
		typedef UInt8 Type;

		enum BitMask : Type
		{
			None,
			Shift = 1 << 0,
			Control = 1 << 1,
			Alt = 1 << 2,
			All = Shift | Control | Alt
		};

		static bool IsValid(const Type KeyMods)
		{
			const bool Result = KeyMods <= EKeyMods::All;
			return Result;
		}

		static const FChar* ToString(const Type KeyMods)
		{
			static_assert(EKeyMods::All == 7, "This table requires updating.");
			static const SizeT ArraySize = EKeyMods::All + 1;
			static const TArray<const FChar* const, ArraySize> LookUpTable = {
				/* 0 */"None",
				/* 1 */"Shift",
				/* 2 */"Ctrl",
				/* 3 */"Shift + Ctrl",
				/* 4 */"Alt",
				/* 5 */"Shift + Alt",
				/* 6 */"Alt + Ctrl",
				/* 7 */"Shift + Ctrl + Alt",
			};

			const FChar* const Result = LookUpTable[KeyMods];
			return Result;
		}
	}
}

#endif
