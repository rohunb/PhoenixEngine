#include "Stdafx.h"
#include "ComponentFilter.h"

using namespace Phoenix;

bool ComponentFilter::PassFilter(const ComponentTypeArray& InComponentTypeArray) const
{
	// Loop through the component type bits
	std::size_t Index = 0;
	for (auto CompId : RequiredComponentsList)
	{
		if (CompId)
		{
			// Check to see if all of the required components are attached to meet the requirements
			if (Index >= InComponentTypeArray.size() || InComponentTypeArray[Index] == false)
			{
				return false;
			}
		}
		Index++;
	}
	Index = 0;
	if (!RequiresOneOfComponentsList.empty())
	{
		for (auto CompId : RequiresOneOfComponentsList)
		{
			if (InComponentTypeArray[Index] != CompId)
			{
				return false;
			}
			++Index;
		}
	}

	Index = 0;
	// Check for clashing components
	if (!ExcludeComponentsList.empty())
	{
		for (auto CompId : ExcludeComponentsList)
		{
			if (InComponentTypeArray[Index] != CompId)
			{
				return false;
			}
		}
	}
	return true;
}

void ComponentFilter::Clear()
{
	RequiredComponentsList.clear();
	RequiresOneOfComponentsList.clear();
	ExcludeComponentsList.clear();
}