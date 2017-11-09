#ifndef PHOENIX_INDEX_SEQUENCE
#define PHOENIX_INDEX_SEQUENCE

#include "Utility/Containers/Tuple.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	template<SizeT... Indices>
	struct TIndexSequence {};

	template<SizeT N, SizeT... Indices>
	struct TMakeIndexSequence : TMakeIndexSequence<N - 1, N - 1, Indices...>
	{};

	template<SizeT... Indices>
	struct TMakeIndexSequence<0, Indices...>
		: TIndexSequence<Indices...>
	{};
}

#endif

