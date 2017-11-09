#ifndef PHOENIX_TYPE_LIST_H
#define PHOENIX_TYPE_LIST_H

#include "Utility/Misc/TypeTraits.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	/*! \brief A container for heterogeneous types
	*
	*	@example struct Type1 {};
	*			 struct Type2 {};
	*			 struct Type3 {};
	*			 
	*			 using TypeList = TTypeList<Type1, Type2, Type3>;
	*/
	template <typename... Types>
	struct TTypeList
	{
		//Number of types
		static constexpr SizeT Size { sizeof...(Types) };
	};

	/////////
	//Count
	/////////

	//0 occurrences
	template <typename TypeToCount, typename TypeList>
	struct TCountImpl : TIntegralConst<SizeT, 0>
	{};

	//Recursive Count, stripping the first type from the type list each recursion
	template <typename TypeToCount, typename TFirst, typename... TOthers>
	struct TCountImpl <TypeToCount, TTypeList<TFirst, TOthers...>>
		: TIntegralConst <SizeT
						 , (TIsSame<TFirst, TypeToCount>::value ? 1 : 0)
						 + TCountImpl < TypeToCount, TTypeList<TOthers...>>::value
						 >
	{};

	/*! \brief Counts the number of occurrences of a type in the type list
	*
	*	@example using TypeList = TTypeList<Type1, Type1, Type2, Type3>;
	*			 TCount<TypeList, Type1>::value == 2
	*/
	template <typename TypeToCount, typename TypeList>
	using TCount = TCountImpl<TypeToCount, TypeList>;


	////////////
	//Contains
	////////////

	/*! \brief Checks if a TypeList contains a given type
	*
	*	@example using TypeList = TTypeList<Type1, Type2, Type3>;
	*			 TContains<TypeList, Type1>::value == true
	*			 TContains<TypeList, OtherType>::value == false
	*/
	template <typename TypeToFind, typename TypeList>
	using TContains = TIntegralConst <bool, (TCount<TypeToFind, TypeList>::value > 0)>;

	////////////
	//Concat
	////////////

	//Empty Lists
	template<typename...>
	struct TConcatImpl
	{
		using Type = TTypeList<>;
	};

	/*! \brief Concatenates multiple TypeLists
	*	@example TConcat<TTypeList<Type1, Type2>, TTypeList<Type3, Type4>> 
				 Will result in: TTypeList<Type1, Type2, Type3, Type4>
	*/
	template<typename... TypeLists>
	using TConcat = typename TConcatImpl<TypeLists...>::Type;

	//1 List
	template<typename... Types>
	struct TConcatImpl<TTypeList<Types...>>
	{
		using Type = TTypeList<Types...>;
	};

	//Multiple Lists. Starts by combining the first two lists and recurses
	template<typename... FirstTypes, typename... NextTypes, typename... TOthers>
	struct TConcatImpl<TTypeList<FirstTypes...>, TTypeList<NextTypes...>, TOthers...>
	{
		using Type = TConcat<TTypeList<FirstTypes..., NextTypes...>, TOthers...>;
	};

	////////////
	//PushBack
	////////////

	/*! \brief Add another Type to the back of the TypeList
	*/
	template<typename TypeList, typename T>
	using TPushBack = TConcat<TypeList, TTypeList<T>>;

	////////////
	//PushFront
	////////////

	/*! \brief Add another Type to the front of the TypeList
	*/
	template<typename TypeList, typename T>
	using TPushFront = TConcat<TTypeList<T>, TypeList>;
}

#endif
