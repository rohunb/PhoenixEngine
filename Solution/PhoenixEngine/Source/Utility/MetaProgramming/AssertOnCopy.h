#pragma once

#include "Utility/Debug/Assert.h"

namespace Phoenix
{
	/*! \brief A Wrapper to contain a type.
		\ Primary use is for a compilation work around with std::function and lambdas.
	*/
	template<typename T>
	class TAssertOnCopy
	{
	public:
		TAssertOnCopy() = default;

		TAssertOnCopy(const TAssertOnCopy&);
		TAssertOnCopy& operator=(const TAssertOnCopy&);

		TAssertOnCopy(TAssertOnCopy&&) = default;
		TAssertOnCopy& operator=(TAssertOnCopy&&) = default;

		T& Get();

		const T& Get() const;

		T& operator*();

		const T& operator*() const;

		T* operator->();

		const T* operator->() const;

	private:
		T Data;
	};

	template<typename T>
	TAssertOnCopy<T>::TAssertOnCopy(const TAssertOnCopy<T>& RHS)
	{
		F_Assert(false, "This class does not support copy operations.");
	}

	template<typename T>
	TAssertOnCopy<T>& TAssertOnCopy<T>::operator=(const TAssertOnCopy<T>& RHS)
	{
		if (this == &RHS)
		{
			return *this;
		}

		F_Assert(false, "This class does not support copy operations.");
		return *this;
	}

	template <class T>
	T& TAssertOnCopy<T>::Get()
	{
		return Data;
	}

	template <class T>
	const T& TAssertOnCopy<T>::Get() const
	{
		return Data;
	}

	template <class T>
	T& TAssertOnCopy<T>::operator*()
	{
		return Data;
	}

	template <class T>
	const T& TAssertOnCopy<T>::operator*() const
	{
		return Data;
	}

	template <class T>
	T* TAssertOnCopy<T>::operator->()
	{
		return &Data;
	}

	template <class T>
	const T* TAssertOnCopy<T>::operator->() const
	{
		return &Data;
	}
}
