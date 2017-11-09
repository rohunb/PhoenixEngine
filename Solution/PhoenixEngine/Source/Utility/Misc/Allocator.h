#ifndef PHOENIX_ALLOCATOR_H
#define PHOENIX_ALLOCATOR_H

#include <memory>
#include <new>

#include "Utility/Debug/Assert.h"
#include "Utility/Debug/Debug.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	template <class T>
	using TAlloc = std::allocator<T>;

	// #FIXME: Implement the tracking functionality for use with the STL.
	template <class T>
	using TTrackedAlloc = TAlloc<T>;

	struct FRawAlloc
	{
		static void* New(SizeT Size);

		static void Delete(void* const Data);
	};

	inline void* FRawAlloc::New(SizeT Size)
	{
		F_Assert(Size, "Size should be greater than zero.");
		if (Size == 0)
		{
			Size = 1;
		}

		void* const Data = operator new(Size);

		F_Assert(Data, "Memory allocation failed.");
		return Data;
	}

	inline void FRawAlloc::Delete(void* const Data)
	{
		F_Assert(Data, "Data should not be null.");
		operator delete(Data);
	}

	namespace EAlignment
	{
		typedef SizeT Type;

		enum Value : Type
		{
			Align4 = 4,
			Align8 = 8,
			Align16 = 16,
			Align32 = 32,
			Align64 = 64,
			Align128 = 128,
			Align256 = 256,
			Align512 = 512,
			Align1024 = 1024,
			Align2048 = 2048,
			Align4096 = 4096
		};

		static bool IsValid(const Type Alignment)
		{
			switch (Alignment)
			{
				case Align4:
				case Align8:
				case Align16:
				case Align32:
				case Align64:
				case Align128:
				case Align256:
				case Align512:
				case Align1024:
				case Align2048:
				case Align4096:
					return true;
			}

			return false;
		}
	}

	struct FRawAlignedAlloc
	{
	public:
		static void* New(const SizeT Size, const EAlignment::Value Alignment);

		static void Delete(void* const Data);

	private:
		struct FPtrValues
		{
			union
			{
				UInt8* DataPtr;
				SizeT DataValue;
			};
		};

		static_assert(sizeof(UInt8*) == sizeof(SizeT), "UInt8* size does not equal SizeT size.");
		static_assert(sizeof(FPtrValues) == sizeof(SizeT), "FPtrValues size does not equal SizeT size.");
	};

	inline void* FRawAlignedAlloc::New(const SizeT Size, const EAlignment::Value Alignment)
	{
		F_Assert(Size, "Size should be greater than zero.");
		F_Assert(EAlignment::IsValid(Alignment), "Alignment is invalid.");

		const SizeT PointerSize = sizeof(UInt8*);
		const SizeT AllocSize = Size + Alignment + PointerSize;

		FPtrValues PtrValues;

		PtrValues.DataPtr = static_cast<UInt8*>(FRawAlloc::New(AllocSize));
		F_Assert(PtrValues.DataPtr, "Memory allocation failed.");

		const SizeT DataValueModAlignment = PtrValues.DataValue % Alignment;
		SizeT UserPtrOffset = Alignment - DataValueModAlignment;

		if (UserPtrOffset < PointerSize)
		{
			// #FIXME: Verify this in 64 bit mode.
			F_DebugBreakOnce();
			UserPtrOffset += Alignment;
		}

		UInt8* const UserPtr = PtrValues.DataPtr + UserPtrOffset;
		UInt8* const DataStartPtrUInt8 = UserPtr - PointerSize;
		SizeT* const DataStartPtr = reinterpret_cast<SizeT*>(DataStartPtrUInt8);

		*DataStartPtr = PtrValues.DataValue;

		F_Assert(UserPtr >= (DataStartPtrUInt8 + PointerSize), "There needs to be additional space for internal use.");
		F_Assert(((SizeT)UserPtr) % Alignment == 0, "User pointer is not aligned according to the alignment specification.");
		F_Assert((UserPtr + Size) <= (PtrValues.DataPtr + AllocSize), "UserPtr can potentially go out of the data bounds.");
		F_Assert(DataStartPtrUInt8 >= PtrValues.DataPtr, "There needs to be additional space for internal use.");

		void* const UserVoidPtr = UserPtr;
		return UserVoidPtr;
	}

	inline void FRawAlignedAlloc::Delete(void* const Data)
	{
		F_Assert(Data, "Data is null.");
		SizeT* InternalDataSizeTPtr = static_cast<SizeT*>(Data);
		--InternalDataSizeTPtr;

		FPtrValues PtrValues;

		PtrValues.DataValue = *InternalDataSizeTPtr;

		F_Assert(PtrValues.DataPtr, "Data is invalid.  Check your allocator usage.");
		F_Assert(PtrValues.DataPtr < Data, "Data is invalid.  Check your allocator usage.");
		F_Assert((PtrValues.DataPtr + sizeof(UInt8*)) <= Data, "Data is invalid.  Check your allocator usage.");

		FRawAlloc::Delete(PtrValues.DataPtr);
	}

	struct FAlloc
	{
	public:
		template <class T>
		static T* New();

		template <class T>
		static T* NewArray(const SizeT Size);

		template <class T>
		static void Delete(T* Data);

		template <class T>
		static void DeleteArray(T* Data);

		template <class T>
		static void SafeDelete(T*& Data);

		template <class T>
		static void SafeDeleteArray(T*& Data);
	};

	template <class T>
	T* FAlloc::New()
	{
		T* Data = new T();

		F_Assert(Data, "Failed to allocate memory.");
		return Data;
	}

	template <class T>
	T* FAlloc::NewArray(const SizeT ArraySize)
	{
		F_Assert(ArraySize, "Size must be a non-zero value.");
		T* Data = new T[ArraySize]();

		F_Assert(Data, "Failed to allocate memory.");
		return Data;
	}

	template <class T>
	void FAlloc::Delete(T* Data)
	{
		delete Data;
	}

	template <class T>
	void FAlloc::DeleteArray(T* Data)
	{
		delete[] Data;
	}

	template <class T>
	void FAlloc::SafeDelete(T*& Data)
	{
		Delete(Data);
		Data = nullptr;
	}

	template <class T>
	void FAlloc::SafeDeleteArray(T*& Data)
	{
		DeleteArray(Data);
		Data = nullptr;
	}

	struct FAlignedAlloc
	{
	public:
		template <class T>
		static T* New(const EAlignment::Value Alignment);

		template <class T>
		static T* NewArray(const SizeT ArrayLength, const EAlignment::Value Alignment);

		template <class T>
		static void Delete(T* Data);

		template <class T>
		static void DeleteArray(T* Data, const SizeT ArrayLength);

		template <class T>
		static void SafeDelete(T*& Data);

		template <class T>
		static void SafeDeleteArray(T*& Data, const SizeT ArrayLength);
	};

	template <class T>
	T* FAlignedAlloc::New(const EAlignment::Value Alignment)
	{
		F_Assert(EAlignment::IsValid(Alignment), "Alignment is invalid.");
		const SizeT AllocSize = sizeof(T);
		void* VoidPtr = FRawAlignedAlloc::New(AllocSize, Alignment);

		F_Assert(VoidPtr, "Failed to allocate memory.");
		T* TPtr = new (VoidPtr) T();

		F_Assert(VoidPtr == TPtr, "Placement new required additional memory overhead.  Rework this.");
		return TPtr;
	}

	template <class T>
	T* FAlignedAlloc::NewArray(const SizeT ArrayLength, const EAlignment::Value Alignment)
	{
		F_Assert(ArrayLength, "ArrayLength should be a non-zero value.");
		F_Assert(EAlignment::IsValid(Alignment), "Alignment is invalid.");
		if (ArrayLength == 0)
		{
			return nullptr;
		}

		const SizeT AllocSize = sizeof(T) * ArrayLength;
		T* TPtr = static_cast<T*>(FRawAlignedAlloc::New(AllocSize, Alignment));

		F_Assert(TPtr, "Failed to allocate memory.");
		for (SizeT I = 0; I < ArrayLength; ++I)
		{
			new (TPtr[I]) T();
		}

		return TPtr;
	}

	template <class T>
	void FAlignedAlloc::Delete(T* Data)
	{
		if (!Data)
		{
			return;
		}

		Data->~T();
		FRawAlignedAlloc::Delete(Data);
	}

	template <class T>
	void FAlignedAlloc::DeleteArray(T* Data, const SizeT ArrayLength)
	{
		F_Assert(ArrayLength, "ArrayLength should be a non-zero value.");
		if (!Data || ArrayLength == 0)
		{
			return;
		}

		for (SizeT I = ArrayLength - 1; I > 0; --I)
		{
			Data[I].~T();
		}

		Data[0].~T();

		FRawAlignedAlloc::Delete(Data);
	}

	template <class T>
	void FAlignedAlloc::SafeDelete(T*& Data)
	{
		Delete(Data);
		Data = nullptr;
	}

	template <class T>
	void FAlignedAlloc::SafeDeleteArray(T*& Data, const SizeT ArrayLength)
	{
		F_Assert(ArrayLength, "ArrayLength should be a non-zero value.");
		DeleteArray(Data, ArrayLength);
		Data = nullptr;
	}
}

#define F_RequireClassHeapAlignment(AlignmentValue)							\
public:																		\
void* operator new(size_t Size)												\
{																			\
	return FRawAlignedAlloc::New(Size, EAlignment::Align##AlignmentValue);	\
}																			\
																			\
void* operator new[](size_t Size)											\
{																			\
	return FRawAlignedAlloc::New(Size, EAlignment::Align##AlignmentValue);	\
}																			\
																			\
void operator delete(void* Data)											\
{																			\
	FRawAlignedAlloc::Delete(Data);											\
}																			\
																			\
void operator delete[](void* Data)											\
{																			\
	FRawAlignedAlloc::Delete(Data);											\
}																			\
private:

#endif
