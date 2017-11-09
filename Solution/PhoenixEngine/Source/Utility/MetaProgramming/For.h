#ifndef PHOENIX_FOR_H
#define PHOENIX_FOR_H

#include "Utility/Containers/Tuple.h"
#include "Utility/MetaProgramming/IndexSequence.h"
#include "Utility/MetaProgramming/Transform.h"
#include "Utility/MetaProgramming/TypeList.h"
#include "Utility/MetaProgramming/TypeWrapper.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/TypeTraits.h"

namespace Phoenix
{
	template<typename TTuple, typename TFunc, SizeT... Indices>
	void ForTupleImpl(TTuple&& Tuple, TFunc&& Func, TIndexSequence<Indices...>)
	{
		//Using an initializer list to expand the indices to call Func(std::get<i>) for each element in the tuple
		auto Expander = { (Func(std::get<Indices>(std::forward<TTuple>(Tuple))), 0)... };
	}

	template<typename... Ts, typename TFunc
			, TEnableIf<(std::tuple_size<TTuple<Ts...>>::value > 0), Int32> = 0>
	void ForTuple(TTuple<Ts...>& Tuple, TFunc&& Func)
	{
		ForTupleImpl(Tuple
					 , std::forward<TFunc>(Func)
					 , TMakeIndexSequence<sizeof...(Ts)>());
	}

	//Tuple of zero elements...don't do anything with it
	template<typename... Ts, typename TFunc
			, TDisableIf < (std::tuple_size<TTuple<Ts...>>::value > 0), Int32> = 0 >
		void ForTuple(TTuple<Ts...>& Tuple, TFunc&& Func)
	{}

	/*! \brief Runs the provided Callable for all Types in the TypeList by instantiating TTypeWrappers containing the type
	*/
	template<typename TypeList, typename TFunc
			, TDisableIf<TypeList::Size == 0, Int32> = 0>
	void ForTypes(TFunc&& Func)
	{
		using TypeWrapperList = TTransform<TypeList, TTypeWrapper>;
		using TupleType = TMakeTuple<TypeWrapperList>;

		ForTupleImpl(TupleType {}
					, std::forward<TFunc>(Func)
					, TMakeIndexSequence<TypeList::Size>());
	}

	template<typename TypeList, typename TFunc
			, TEnableIf<TypeList::Size == 0, Int32> = 0>
	void ForTypes(TFunc&& Func)
	{
		//Do nothing for an empty TypeList
	}
}

#endif
