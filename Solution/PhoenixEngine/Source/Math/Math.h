#ifndef PHOENIX_MATH_H
#define PHOENIX_MATH_H

#include <cmath>

#include "Utility/Debug/Assert.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	template <class T>
	struct TMath
	{
		static T Abs(const T Value);
		static T Clamp(const T Value, const T Min, const T Max);
		static bool IsCloseTo(const T Value, const T DesiredValue, const T AcceptableRange = 0.000001);
		static bool IsInRange(const T Value, const T Min, const T Max);
		static T Min(const T LHS, const T RHS);
		static T Max(const T LHS, const T RHS);
		static T Modulo(const T Value, const T Mod);
	};

	typedef TMath<Float32> FMathf;
	typedef TMath<Float64> FMathd;

	template <class T>
	T TMath<T>::Abs(const T Value)
	{
		const T Result = std::abs(Value);
		return Result;
	}

	template <class T>
	T TMath<T>::Clamp(const T Value, const T Min, const T Max)
	{
		F_Assert(Min <= Max, "Invalid values, min: " << Min << ", max: " << Max);
		const T Result = 
			Value < Min ? Min :
			Value > Max ? Max :
			Value;
		return Result;
	}

	template <class T>
	bool TMath<T>::IsCloseTo(const T Value, const T DesiredValue, const T AcceptableRange)
	{
		const T Delta = DesiredValue - Value;
		const T AbsDelta = TMath<T>::Abs(Delta);

		const bool Result = AbsDelta <= AcceptableRange;
		return Result;
	}

	template <class T>
	bool TMath<T>::IsInRange(const T Value, const T Min, const T Max)
	{
		const bool Result = TMath<T>::Clamp(Value, Min, Max) == Value;
		return Result;
	}

	template <class T>
	T TMath<T>::Min(const T LHS, const T RHS)
	{
		const T Result = LHS < RHS ? LHS : RHS;
		return Result;
	}

	template <class T>
	T TMath<T>::Max(const T LHS, const T RHS)
	{
		const T Result = LHS > RHS ? LHS : RHS;
		return Result;
	}

	template <class T>
	T TMath<T>::Modulo(const T Value, const T Mod)
	{
		const T Result = std::fmod(Value, Mod);
		return Result;
	}
}

#endif
