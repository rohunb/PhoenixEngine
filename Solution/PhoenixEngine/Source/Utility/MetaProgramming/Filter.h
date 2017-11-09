#ifndef PHOENIX_FILTER_H
#define PHOENIX_FILTER_H

#include "Utility/MetaProgramming/TypeList.h"
#include "Utility/Misc/TypeTraits.h"

namespace Phoenix
{
	//Empty List
	template<typename, template <typename> class>
	struct TFilterImpl
	{
		using Type = TTypeList<>;
	};

	/*! \brief Results in a TypeList containing only the Types that pass the Filter 
	*	
	*	@example TFilter<TTypeList<Int32, UInt8, Float32>, TIsIntegral>
	*			 Results in: TTypeList<Int32, UInt8>
	*/
	template<typename TypeList, template <typename> class TFilterTrait>
	using TFilter = typename TFilterImpl<TypeList, TFilterTrait>::Type;

	//Recurse, Concat-ing matching types and stripping the first type each time
	template<typename TFirst, typename... TOthers, template <typename> class TFilterTrait>
	struct TFilterImpl<TTypeList<TFirst, TOthers...>, TFilterTrait>
	{
		using TNext = TFilter<TTypeList<TOthers...>, TFilterTrait>;

		using Type = std::conditional_t < TFilterTrait<TFirst>::value
										, TConcat<TTypeList<TFirst>, TNext>
										, TNext
										>;
	};
}

#endif
