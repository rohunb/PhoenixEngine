#pragma once

#include "Utility/Debug/Debug.h"
#include "Utility/Handle/Handle.h"
#include "Utility/Handle/HandleData.h"

namespace Phoenix
{
	template <class T>
	class THandles
	{
	public:
		typedef THandle<T> HandleT;

		THandles() = default;

		THandles(const THandles&) = delete;
		THandles& operator=(const THandles&) = delete;

		THandles(THandles&&) = delete;
		THandles& operator=(THandles&&) = delete;

		template <class... Arguments>
		THandle<T> CreateHandle(Arguments&&... Args);

		const TVector<T>& GetData() const;

		SizeT GetDataSize() const;

		T& operator[](const SizeT Index);

		const T& operator[](const SizeT Index) const;

	protected:
	private:
		THandlesData<T> HandlesData;
	};

	template <class T>
	template <class... Arguments>
	THandle<T> THandles<T>::CreateHandle(Arguments&&... Args)
	{
		THandle<T> Handle;
		
		const SizeT DataIndex = HandlesData.Data.size();
		HandlesData.Data.push_back(T(std::forward<Arguments>(Args)...));

		const SizeT UnusedHIDataSize = HandlesData.UnusedHIData.size();

		const bool CanReuseHIData = UnusedHIDataSize != 0;
		if (CanReuseHIData)
		{
			const SizeT HIDataIndex = HandlesData.UnusedHIData[UnusedHIDataSize - 1];
			HandlesData.UnusedHIData.pop_back();
			
			THandleItemData& ReusedHIData = HandlesData.HIData[HIDataIndex];
			
			ReusedHIData.DataIndex = DataIndex;
			ReusedHIData.RefCount = 1;

			Handle.Init(&HandlesData, HIDataIndex);
			return Handle;
		}

		THandleItemData NewHIData;

		NewHIData.DataIndex = DataIndex;
		NewHIData.RefCount = 1;

		const SizeT HIDataIndex = HandlesData.HIData.size();
		HandlesData.HIData.emplace_back(std::move(NewHIData));

		Handle.Init(&HandlesData, HIDataIndex);
		return Handle;
	}

	template <class T>
	const TVector<T>& THandles<T>::GetData() const
	{
		return HandlesData.Data;
	}

	template <class T>
	SizeT THandles<T>::GetDataSize() const
	{
		const SizeT DataSize = HandlesData.Data.size();
		return DataSize;
	}

	template <class T>
	T& THandles<T>::operator[](const SizeT Index)
	{
		T& Item = HandlesData.Data[Index];
		return Item;
	}

	template <class T>
	const T& THandles<T>::operator[](const SizeT Index) const
	{
		const T& Item = HandlesData.Data[Index];
		return Item;
	}
}
