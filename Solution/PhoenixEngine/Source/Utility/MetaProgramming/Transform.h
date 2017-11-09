#ifndef PHOENIX_TRANSFORM_H
#define PHOENIX_TRANSFORM_H

#include "Utility/MetaProgramming/TypeList.h"

namespace Phoenix
{
	//Empty List case
	template<typename, template<typename> class>
	struct TTransformImpl
	{
		using Type = TTypeList<>;
	};

	/*! \brief Apply a transformation to all Types in the TypeList
	*	
	*	@example TTransform<TTypeList<Int32, Float32>, std::add_const_t>
	*			 Results in: TTypeList<const Int32, const Float32>
	*/
	template<typename TypeList, template<typename> class TransformTrait>
	using TTransform = typename TTransformImpl<TypeList, TransformTrait>::Type;

	//Recurse, Transforming each Type in the List
	template<typename TFirst, typename... TOthers, template<typename> class TransformTrait>
	struct TTransformImpl<TTypeList<TFirst, TOthers...>, TransformTrait>
	{
		using Type = TConcat <TTypeList<TransformTrait<TFirst>>
							, TTransform<TTypeList<TOthers...>, TransformTrait >>;

	};
}

#endif
