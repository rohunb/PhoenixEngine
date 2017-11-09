#ifndef PHOENIX_HAS_METHOD_H
#define PHOENIX_HAS_METHOD_H

#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/TypeTraits.h"

namespace Phoenix
{
	/*! \brief Defines a trait named THasMethod_MethodName<Type, Args...>
	*	\ It will test if Type has a Method defined as MethodName(Args&...)
	*/
	#define F_DefineTrait_HasMethod(MethodName)																\
	template<typename Type, typename... TArgs>																\
	struct THasMethod_##MethodName																			\
	{																										\
		/*Simulates a call to the MethodName on Type*/														\
		template<typename T, typename... As>																\
		static auto CallMemberMethod(T& Object, As&... Args)												\
			-> decltype(Object.MethodName(Args...));														\
																											\
		/*Will be picked if the above call can succeed*/													\
		template<typename T, typename... As>																\
		static auto Test(int) 																				\
			-> decltype(CallMemberMethod(std::declval<T&>(), std::declval<As&>()...), std::true_type {});	\
																											\
		/*Fallback...the Method call failed*/																\
		template<typename, typename...>																		\
		static std::false_type Test(...);																	\
																											\
		/*Result of the test*/																				\
		static const bool Value {std::is_same<decltype(Test<Type, TArgs...>(0)), std::true_type>::value};	\
	};			

}

#endif
