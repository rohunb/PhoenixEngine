#ifndef PHOENIX_INDEX_OF_H
#define PHOENIX_INDEX_OF_H

#include "Utility/MetaProgramming/TypeList.h"

namespace Phoenix
{
	/*! @brief Calculates the Index (zero based) of a Type in a List of Types.
	*	@warning Will static_assert if Type is not present. Use TContains for validation.
	*
	*	@example using TypeList = TTypeList<Type1, Type2, Type3>;
	*			 TIndexOf<Type1, TypeList>::value == 0;
	*/
	template <typename TypeToFind, typename... TypeList>
	struct TIndexOfImpl
	{
		static_assert(TContains<TTypeList<TypeList...>, TypeToFind>::value, "\n\n**************Type is not present in list****************\n\n");
	};

	// Found the type
	template <typename TypeToFind, typename... TypeList>
	struct TIndexOfImpl <TypeToFind, TTypeList<TypeToFind, TypeList...>>
		: TIntegralConst<SizeT, 0>
	{};

	// Recurse: Strip the first Type and keep looking for the Type
	template <typename TypeToFind, typename TFirst, typename... TOthers>
	struct TIndexOfImpl <TypeToFind, TTypeList<TFirst, TOthers...>>
		: TIntegralConst< SizeT
						, 1 + TIndexOfImpl<TypeToFind, TTypeList<TOthers...>>::value 
						>
	{};

	template <typename TypeToFind, typename... TypeList>
	using TIndexOf = TIndexOfImpl<TypeToFind, TypeList...>;

}


#endif