#pragma once

#include <chrono>

#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/TypeTraits.h"

namespace Phoenix
{
	class FHighResolutionTimer
	{
	public:
		void Reset();

		void Update();

		template <typename T = Float32>
		T GetDeltaSeconds();

		template <typename T = Float32>
		T GetCachedCurrentTime() const;

		template <typename T = Float32>
		T GetCachedPreviousTime() const;

		template <typename T = Float32>
		static T GetTimeInSeconds();

	private:
		typedef std::chrono::high_resolution_clock FHighResClock;
		typedef std::chrono::time_point<FHighResClock> FHighResTimePoint;

		typedef Float32 FTimeStamp;

		FTimeStamp CurrentTime;
		FTimeStamp PreviousTime;
	};

	inline void FHighResolutionTimer::Reset()
	{
		const FTimeStamp TimeStamp = GetTimeInSeconds();

		CurrentTime = TimeStamp;
		PreviousTime = TimeStamp;
	}

	inline void FHighResolutionTimer::Update()
	{
		const FTimeStamp TimeStamp = GetTimeInSeconds();

		PreviousTime = CurrentTime;
		CurrentTime = TimeStamp;
	}

	template <typename T>
	T FHighResolutionTimer::GetDeltaSeconds()
	{
		static_assert(TIsFloatingPoint<T>::value, "This template must be a floating point type.");

		const T DeltaSeconds = static_cast<T>(CurrentTime - PreviousTime);
		return DeltaSeconds;
	}

	template <typename T>
	T FHighResolutionTimer::GetCachedCurrentTime() const
	{
		static_assert(TIsFloatingPoint<T>::value, "This template must be a floating point type.");

		const T CachedCurrentTime = static_cast<T>(CurrentTime);
		return CachedCurrentTime;
	}

	template <typename T>
	T FHighResolutionTimer::GetCachedPreviousTime() const
	{
		static_assert(TIsFloatingPoint<T>::value, "This template must be a floating point type.");

		const T CachedPreviousTime = static_cast<T>(PreviousTime);
		return CachedPreviousTime;
	}

	template <typename T>
	T FHighResolutionTimer::GetTimeInSeconds()
	{
		static_assert(TIsFloatingPoint<T>::value, "This template must be a floating point type.");
		static auto StartTime = FHighResClock::now();

		const FHighResTimePoint TimeStamp = FHighResClock::now();
		const auto Duration = TimeStamp - StartTime;

		const T DeltaSeconds = std::chrono::duration_cast<std::chrono::duration<T>>(Duration).count();
		return DeltaSeconds;
	}
}
