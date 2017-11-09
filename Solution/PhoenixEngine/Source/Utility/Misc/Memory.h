#ifndef PHOENIX_MEMORY_H
#define PHOENIX_MEMORY_H

#include <memory>

#include "Utility/Debug/Assert.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	template<class T>
	using TUniquePtr = std::unique_ptr<T>;

	template <class T>
	using TSharedPtr = std::shared_ptr<T>;

	template <class T>
	using TWeakPtr = std::weak_ptr<T>;

	template <class T>
	class TRawPtr
	{
	public:
		TRawPtr<T>() = default;

		TRawPtr<T>(const TRawPtr&);
		TRawPtr<T>& operator=(const TRawPtr&);

		TRawPtr<T>(TRawPtr&&);
		TRawPtr<T>& operator=(TRawPtr&&);

		TRawPtr<T>(T* InPtr);
		TRawPtr<T>& operator=(T* InPtr);

		TRawPtr<T>(const NullptrT InPtr);
		TRawPtr<T>& operator=(const NullptrT InPtr);

		bool IsValid() const;

		const T* const & Get() const;

		T*& Get();

		const T& operator*() const;

		T& operator*();

		const T* operator->() const;

		T* operator->();

		TRawPtr<T>& operator++();

		TRawPtr<T>& operator--();

		TRawPtr<T> operator++(Int32);

		TRawPtr<T> operator--(Int32);

		const T& operator[](const SizeT Index) const;

		T& operator[](const SizeT Index);

		bool operator==(const TRawPtr<T>& RHS) const;

		bool operator!=(const TRawPtr<T>& RHS) const;

	protected:
	private:
		T* Ptr{ nullptr };

		void PostMoveReset();
	};

#pragma region TRawPtr Implementation

	template <class T>
	TRawPtr<T>::TRawPtr(const TRawPtr<T>& RHS)
		: Ptr(RHS.Ptr)
	{
	}

	template <class T>
	TRawPtr<T>& TRawPtr<T>::operator=(const TRawPtr<T>& RHS)
	{
		if (this == &RHS)
		{
			return *this;
		}

		Ptr = RHS.Ptr;
		return *this;
	}

	template <class T>
	TRawPtr<T>::TRawPtr(TRawPtr&& RHS)
		: Ptr(RHS.Ptr)
	{
		RHS.PostMoveReset();
	}

	template <class T>
	TRawPtr<T>& TRawPtr<T>::operator=(TRawPtr&& RHS)
	{
		if (this == &RHS)
		{
			return *this;
		}

		Ptr = RHS.Ptr;
		RHS.PostMoveReset();

		return *this;
	}

	template <class T>
	TRawPtr<T>::TRawPtr(T* InPtr)
		: Ptr(InPtr)
	{
	}

	template <class T>
	TRawPtr<T>& TRawPtr<T>::operator=(T* InPtr)
	{
		Ptr = InPtr;
		return *this;
	}

	template <class T>
	TRawPtr<T>::TRawPtr(const NullptrT InPtr)
	{
		Ptr = InPtr;
	}

	template <class T>
	TRawPtr<T>& TRawPtr<T>::operator=(const NullptrT InPtr)
	{
		Ptr = InPtr;
		return *this;
	}

	template <class T>
	bool TRawPtr<T>::IsValid() const
	{
		const bool bIsValid = Ptr != nullptr;
		return bIsValid;
	}

	template <class T>
	const T* const & TRawPtr<T>::Get() const
	{
		return Ptr;
	}

	template <class T>
	T*& TRawPtr<T>::Get()
	{
		return Ptr;
	}

	template <class T>
	const T& TRawPtr<T>::operator*() const
	{
		F_Assert(Ptr, "Ptr is null.");
		return *Ptr;
	}

	template <class T>
	T& TRawPtr<T>::operator*()
	{
		F_Assert(Ptr, "Ptr is null.");
		return *Ptr;
	}

	template <class T>
	const T* TRawPtr<T>::operator->() const
	{
		F_Assert(Ptr, "Ptr is null.");
		return Ptr;
	}

	template <class T>
	T* TRawPtr<T>::operator->()
	{
		F_Assert(Ptr, "Ptr is null.");
		return Ptr;
	}

	template <class T>
	TRawPtr<T>& TRawPtr<T>::operator++()
	{
		F_Assert(Ptr, "Ptr is null.");
		++Ptr;
		return *this;
	}

	template <class T>
	TRawPtr<T>& TRawPtr<T>::operator--()
	{
		F_Assert(Ptr, "Ptr is null.");
		--Ptr;
		return *this;
	}

	template <class T>
	TRawPtr<T> TRawPtr<T>::operator++(Int32)
	{
		F_Assert(Ptr, "Ptr is null.");
		TRawPtr<T> Original(*this);
		operator++();
		return Original;
	}

	template <class T>
	TRawPtr<T> TRawPtr<T>::operator--(Int32)
	{
		F_Assert(Ptr, "Ptr is null.");
		TRawPtr<T> Original(*this);
		operator--();
		return Original;
	}

	template <class T>
	const T& TRawPtr<T>::operator[](const SizeT Index) const
	{
		F_Assert(Ptr, "Ptr is null.");
		return Ptr[Index];
	}

	template <class T>
	T& TRawPtr<T>::operator[](const SizeT Index)
	{
		F_Assert(Ptr, "Ptr is null.");
		return Ptr[Index];
	}

	template <class T>
	bool TRawPtr<T>::operator==(const TRawPtr<T>& RHS) const
	{
		const bool IsEqual = Ptr == RHS.Ptr;
		return IsEqual;
	}

	template <class T>
	bool TRawPtr<T>::operator!=(const TRawPtr<T>& RHS) const
	{
		const bool IsNotEqual = !(*this == RHS);
		return IsNotEqual;
	}

	template <class T>
	void TRawPtr<T>::PostMoveReset()
	{
		Ptr = nullptr;
	}

#pragma endregion
}

#endif
