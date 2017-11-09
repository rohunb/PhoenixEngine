#ifndef PHOENIX_RANDOM_H
#define PHOENIX_RANDOM_H

#include <random>
#include "Math/Vector3D.h"
#include "Utility/Debug/Assert.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/TypeTraits.h"
#include "Utility/Misc/String.h"

namespace Phoenix
{
	class FRandom
	{
	public:
		bool Bool();
		bool Bool(Float64 TrueProbability);

		//Floating point type between 0 & 1
		template<typename T = Float32>
		T UnitFloat();

		//[0, Max)
		template<typename T>
		T Bounded(const T& Max);

		//[Min, Max)
		template<typename T>
		T Range(const T& Min, const T& Max);

		//[char::min, char::max). Not guaranteed to be alphabet.
		FString String(UInt32 Size);

		// #FIXME: Fix the cyclical layer dependency here.
		FVector3D UnitVector3();

	private:
		std::mt19937 RandomEngine { std::random_device()() };

		//1 Byte types require special handling
		template<typename T
				, TEnableIf<TIsIntegral<T>::value
							&& (sizeof(T) == sizeof(FChar))
							&& std::is_unsigned<T>::value, Int32> = 0>
		T RandomRangeImpl(const T& Min, const T& Max);

		template<typename T
				, TEnableIf<TIsIntegral<T>::value
							&& (sizeof(T) == sizeof(FChar))
							&& std::is_signed<T>::value, Int32> = 0>
		T RandomRangeImpl(const T& Min, const T& Max);
		
		//Integer types
		template<typename T
				, TEnableIf<TIsIntegral<T>::value
							&& (sizeof(T) > sizeof(FChar)), Int32> = 0>
		T RandomRangeImpl(const T& Min, const T& Max);

		//Floating Point types
		template<typename T
				, TEnableIf<TIsFloatingPoint<T>::value, Int32> = 0>
		T RandomRangeImpl(const T& Min, const T& Max);
	};

	template<typename T>
	T FRandom::UnitFloat()
	{
		static_assert(TIsFloatingPoint<T>::value, "FRandom::UnitFloat() will only return 0 for Ints\n");
		return RandomRangeImpl(0.f, 1.f);
	}

	template<typename T>
	T FRandom::Bounded(const T& Max)
	{
		return RandomRangeImpl(T {}, Max);
	}

	template<typename T>
	T FRandom::Range(const T& Min, const T& Max)
	{
		//#FIXME Determine if this Assert is required or if it's handled automatically
		F_Assert(Min <= Max, "Min should be less than Max");

		return RandomRangeImpl(Min, Max);
	}

	template<typename T
			, TEnableIf<TIsIntegral<T>::value
						&& (sizeof(T) == sizeof(FChar))
						&& std::is_unsigned<T>::value, Int32>>
	T FRandom::RandomRangeImpl(const T& Min, const T& Max)
	{
		std::uniform_int_distribution<UInt16> Distribution { Min, Max };
		return static_cast<T>(Distribution(RandomEngine));
	}
	
	template<typename T
			, TEnableIf<TIsIntegral<T>::value
						&& (sizeof(T) == sizeof(FChar))
						&& std::is_signed<T>::value, Int32>>
	T FRandom::RandomRangeImpl(const T& Min, const T& Max)
	{
		std::uniform_int_distribution<Int16> Distribution { Min, Max };
		return static_cast<T>(Distribution(RandomEngine));
	}

	//Integer types
	template<typename T
			, TEnableIf<TIsIntegral<T>::value
						&& (sizeof(T) > sizeof(FChar)), Int32>>
	T FRandom::RandomRangeImpl(const T& Min, const T& Max)
	{
		std::uniform_int_distribution<T> Distribution { Min, Max };
		return Distribution(RandomEngine);
	}

	//Floating Point types
	template<typename T
			, TEnableIf<TIsFloatingPoint<T>::value, Int32>>
	T FRandom::RandomRangeImpl(const T& Min, const T& Max)
	{
		std::uniform_real_distribution<T> Distribution { Min, Max };
		return Distribution(RandomEngine);
	}
}

#endif