#ifndef PHOENIX_REPEAT_H
#define PHOENIX_REPEAT_H

#include "Utility/MetaProgramming/TypeList.h"

namespace Phoenix
{
	//Recurse, pushing another type back to the list each time
	template<typename T, SizeT Reps>
	struct TRepeatImpl
	{
		using Type = TPushBack<typename TRepeatImpl<T, Reps - 1>::Type, T>;
	};

	//0 reps
	template<typename T>
	struct TRepeatImpl<T, 0>
	{
		using Type = TTypeList<>;
	};

	/*! \brief Generates a TypeList with the given type repeated Reps times
	*	
	*	@example: TRepeat<T, 3> generates:
	*			  TTypeList<T, T, T>
	*/
	template<typename T, SizeT Reps>
	using TRepeat = typename TRepeatImpl<T, Reps>::Type;
}

#endif
