#pragma once
#ifndef PHOENIX_COMPONENT_MANAGER_CONFIG_H
#define PHOENIX_COMPONENT_MANAGER_CONFIG_H

#include "Utility/Containers/BitArray.h"
#include "Utility/MetaProgramming/TypeList.h"
#include "Utility/MetaProgramming/IndexOf.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	/*! \brief Configuration for the Component Manager. 
	*	\ Will contain the TypeList of Components, Tags, Requirements and Systems
	*/
	template<typename TComponentList, typename TTagList, typename TRequirementsList, typename TSystemList>
	struct TComponentManagerConfig
	{
		using ComponentList = TComponentList;
		using TagList = TTagList;
		using RequirementList = TRequirementsList;
		using SystemList = TSystemList;

		template<typename TComponent>
		static constexpr bool IsComponent()
		{
			return TContains<TComponent, ComponentList>::value;
		}

		template<typename TTag>
		static constexpr bool IsTag()
		{
			return TContains<TTag, TagList>::value;
		}

		template<typename TRequirement>
		static constexpr bool IsRequirement()
		{
			return TContains<TRequirement, RequirementList>::value;
		}
		
		static constexpr SizeT GetComponentCount()
		{
			return ComponentList::Size;
		}

		static constexpr SizeT GetTagCount()
		{
			return TagList::Size;
		}

		static constexpr SizeT GetRequirementCount()
		{
			return RequirementList::Size;
		}

		template<typename TComponent>
		static constexpr SizeT GetComponentID()
		{
			return TIndexOf<TComponent, ComponentList>::value;
		}

		template<typename TTag>
		static constexpr SizeT GetTagID()
		{
			return TIndexOf<TTag, TagList>::value;
		}
		
		template<typename TRequirement>
		static constexpr SizeT GetRequirementID()
		{
			return TIndexOf<TRequirement, RequirementList>::value;
		}

		//The Bit array will represent both components and tags, with the components first and tags following
		using ComponentsBitArray = TBitArray<ComponentList::Size + TagList::Size>;

		template<typename TComponent>
		static constexpr SizeT GetComponentBit()
		{
			return GetComponentID<TComponent>();
		}

		template<typename TTag>
		static constexpr SizeT GetTagBit()
		{
			//Tags are after all the Components
			return GetComponentCount() + GetTagID<TTag>();
		}
	};
}

#endif
