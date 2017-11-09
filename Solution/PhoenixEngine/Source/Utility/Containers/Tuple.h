#ifndef PHOENIX_TUPLE_H
#define PHOENIX_TUPLE_H

#include <tuple>
#include "Utility/MetaProgramming/Rename.h"

namespace Phoenix
{
	template<typename... Ts>
	using TTuple = std::tuple<Ts...>;

	template<typename TypeList>
	using TMakeTuple = TRename<TypeList, TTuple>;
}

#endif
