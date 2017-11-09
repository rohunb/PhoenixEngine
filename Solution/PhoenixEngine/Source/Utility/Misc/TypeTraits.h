#ifndef PHOENIX_TYPE_TRAITS_H
#define PHOENIX_TYPE_TRAITS_H

#include <type_traits>

namespace Phoenix
{
	typedef std::true_type FTrueType;
	typedef std::false_type FFalseType;

	template<bool Test, typename T = void>
	using TEnableIf = std::enable_if_t<Test, T>;

	template<bool Test, typename T = void>
	using TDisableIf = std::enable_if_t<!Test, T>;

	template<typename T1, typename T2>
	using TIsSame = std::is_same<T1, T2>;

	template<typename T>
	using TIsArithmetic = std::is_arithmetic<T>;

	template<typename T>
	using TIsIntegral = std::is_integral<T>;

	template<typename T>
	using TIsFloatingPoint = std::is_floating_point<T>;

	template<typename T, T Value>
	using TIntegralConst = std::integral_constant<T, Value>;
}

#endif
