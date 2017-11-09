#ifndef PHOENIX_HAS_INNER_TYPE_H
#define PHOENIX_HAS_INNER_TYPE_H

#include "Utility/Misc/TypeTraits.h"

namespace Phoenix
{
	/*! \brief Defines a trait named THasHasInnerType_InnerTypeName<Type>
	*	\ It will test if Type has an inner type named InnerTypeName (typedef / using / inner class)
	*/
	#define F_DefineTrait_HasInnerType(InnerTypeName)														\
																											\
	template<typename TypeToCheck>																			\
	struct THasInnerType_##InnerTypeName																	\
	{																										\
		/*Will Return True Type if T::InnerTypeName is well formed*/										\
		template<typename T>																				\
		static auto Test(typename T::InnerTypeName*)->FTrueType;											\
																											\
		/*InnerType not found*/																				\
		template<typename>																					\
		static FFalseType Test(...);																		\
																											\
		/*Holds the result of the test*/																	\
		static const bool Value { TIsSame<decltype(Test<TypeToCheck>(nullptr)), FTrueType>::value };		\
	};
}
#endif
