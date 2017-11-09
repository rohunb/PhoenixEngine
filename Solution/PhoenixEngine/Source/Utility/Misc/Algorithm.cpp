#include "Stdafx.h"
#include "Utility/Misc/Algorithm.h"

#include <cstdlib>

namespace Phoenix
{
	int IntFromString(const FString& InStr)
	{
		return std::atoi(InStr.c_str());
	}
}