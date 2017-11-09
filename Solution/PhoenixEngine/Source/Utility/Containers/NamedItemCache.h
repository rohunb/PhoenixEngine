#ifndef PHOENIX_NAMED_ITEM_CACHE_H
#define PHOENIX_NAMED_ITEM_CACHE_H

#include "Utility/Containers/Array.h"
#include "Utility/Containers/UnorderedMap.h"
#include "Utility/Debug/Debug.h"
#include "Utility/Handle/Handles.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	template <class T>
	class TNamedItemCache
	{
	public:
		typedef FString KeyT;
		typedef T ValueT;

		bool AddEntry(const KeyT& Key, const ValueT& Value);

		bool AddEntry(KeyT&& Key, ValueT&& Value);

		void Clear();

		T* GetItem(const KeyT& Key);

		const T* GetItem(const KeyT& Key) const;

		T& GetItem(const KeyT& Key, T& DefaultReturnValue);

		const T& GetItem(const KeyT& Key, const T& DefaultReturnValue) const;

		T GetItemCopy(const KeyT& Key);

		bool HasItem(const KeyT& Key) const;

		SizeT GetItemCount() const;

		bool RemoveItem(const KeyT& Key);

	protected:
	private:
		typedef TUnorderedMap<KeyT, ValueT> ItemsT;

		ItemsT Items;
	};

	template <class T>
	bool TNamedItemCache<T>::AddEntry(const KeyT& Key, const ValueT& Value)
	{
		F_Assert(Key.size(), "Key has a size of zero.");

		const auto Pair = Items.emplace(Key, Value);
		const bool WasEntryAdded = Pair.second;
		return WasEntryAdded;
	}

	template <class T>
	bool TNamedItemCache<T>::AddEntry(KeyT&& Key, ValueT&& Value)
	{
		F_Assert(Key.size(), "Key has a size of zero.");

		const auto Iter = Items.find(Key);
		if (Iter != Items.end())
		{
			return false;
		}

		const auto Pair = Items.emplace(std::move(Key), std::move(Value));
		const bool WasEntryAdded = Pair.second;
		return WasEntryAdded;
	}

	template <class T>
	void TNamedItemCache<T>::Clear()
	{
		Items.clear();
	}

	template <class T>
	T* TNamedItemCache<T>::GetItem(const KeyT& Key)
	{
		F_Assert(Key.size(), "Key has a size of zero.");

		const auto Iter = Items.find(Key);
		const bool WasItemFound = Iter != Items.end();

		if (WasItemFound)
		{
			T* const Value = &Iter->second;
			return Value;
		}

		return nullptr;
	}

	template <class T>
	const T* TNamedItemCache<T>::GetItem(const KeyT& Key) const
	{
		F_Assert(Key.size(), "Key has a size of zero.");

		const auto Iter = Items.find(Key);
		const bool WasItemFound = Iter != Items.end();

		if (WasItemFound)
		{
			const T* const Value = &Iter->second;
			return Value;
		}

		return nullptr;
	}

	template <class T>
	T& TNamedItemCache<T>::GetItem(const KeyT& Key, T& DefaultReturnValue)
	{
		F_Assert(Key.size(), "Key has a size of zero.");

		T* Result = GetItem(Key);
		if (!Result)
		{
			return DefaultReturnValue;
		}

		return *Result;
	}

	template <class T>
	const T& TNamedItemCache<T>::GetItem(const KeyT& Key, const T& DefaultReturnValue) const
	{
		F_Assert(Key.size(), "Key has a size of zero.");

		const T* const Result = GetItem(Key);
		if (!Result)
		{
			return DefaultReturnValue;
		}

		return *Result;
	}

	template <class T>
	T TNamedItemCache<T>::GetItemCopy(const KeyT& Key)
	{
		T* Item = GetItem(Key);
		if (!Item)
		{
			T DefaultReturnValue;
			return DefaultReturnValue;
		}

		return *Item;
	}

	template <class T>
	bool TNamedItemCache<T>::HasItem(const KeyT& Key) const
	{
		F_Assert(Key.size(), "Key has a size of zero.");

		const auto Iter = Items.find(Key);
		const bool WasItemFound = Iter != Items.end();
		return WasItemFound;
	}

	template <class T>
	SizeT TNamedItemCache<T>::GetItemCount() const
	{
		const SizeT ItemCount = Items.size();
		return ItemCount;
	}

	template <class T>
	bool TNamedItemCache<T>::RemoveItem(const KeyT& Key)
	{
		F_Assert(Key.size(), "Key has a size of zero.");

		const auto Iter = Items.erase(Key);
		const bool WasItemRemoved = Iter == 1;
		return WasItemRemoved;
	}
}

#endif

