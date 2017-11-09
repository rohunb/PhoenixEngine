#pragma once

#include "Utility/Containers/Vector.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	struct THandleItemData
	{
		SizeT DataIndex{ TNumericLimits<SizeT>::max() };
		SizeT RefCount{ 0 };
	};

	template <class T>
	struct THandlesData
	{
		typedef TVector<T> DataT;
		typedef TVector<THandleItemData> HIDataT;
		typedef TVector<SizeT> UnusedHIDataT;

		DataT Data;
		HIDataT HIData;
		UnusedHIDataT UnusedHIData;
	};
}

