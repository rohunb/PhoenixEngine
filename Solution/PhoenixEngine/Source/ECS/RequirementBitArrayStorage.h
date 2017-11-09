#pragma once
#ifndef PHOENIX_REQUIREMENT_BIT_ARRAY_STORAGE_H
#define PHOENIX_REQUIREMENT_BIT_ARRAY_STORAGE_H

#include "Utility/Containers/Tuple.h"
#include "Utility/MetaProgramming/Filter.h"
#include "Utility/MetaProgramming/For.h"
#include "Utility/MetaProgramming/Repeat.h"
#include "Utility/Misc/TypeTraits.h"

namespace Phoenix
{
	/* \brief Stores requirement BitArrays in a Tuple, that correspond to each type of requirement
	*/
	template<typename TConfig>
	struct TRequirementBitArrayStorage
	{
	private:
		using TRequirementList = typename TConfig::RequirementList;

		using TRequirementBitArray = typename TConfig::ComponentsBitArray;

		//List of BitArrays for each Requirement
		using TBitArrayList = TRepeat<TRequirementBitArray, TConfig::GetRequirementCount()>;

		//Essentially a table of the different Requirements
		using TBitArrayTuple = TMakeTuple<TBitArrayList>;

		//Used to get only Components from a Requirement, which contains Tags as well
		template<typename TComponent>
		using TComponentFilter = TIntegralConst<bool, TConfig::template IsComponent<TComponent>()>;

		template<typename TTag>
		using TTagFilter = TIntegralConst<bool, TConfig::template IsTag<TTag>()>;

	public:
		//Get all the Components from the Requirement
		template<typename TRequirement>
		using TComponentsInRequirement = TFilter<TRequirement, TComponentFilter>;

		//All the tags
		template<typename TRequirement>
		using TTagsInRequirement = TFilter<TRequirement, TTagFilter>;

		TRequirementBitArrayStorage()
		{
			ForTypes<TRequirementList>
				([this](auto Requirement)
				{
					using RequirementType = typename decltype(Requirement)::Type;
					this->InitializeBitArray<RequirementType>();
				});
		}

		//Returns the Bit Array corresponding to the Requirement
		template<typename TRequirement>
		const TRequirementBitArray& GetRequirementBitArray() const
		{
			static_assert(TConfig::template IsRequirement<TRequirement>(), "Not a Requirement");

			constexpr SizeT RequirementID = TConfig::template GetRequirementID<TRequirement>();

			return std::get<RequirementID>(BitArrayStorage);
		}

		template<typename TRequirement>
		TRequirementBitArray& GetRequirementBitArray()
		{
			static_assert(TConfig::template IsRequirement<TRequirement>(), "Not a Requirement");

			constexpr SizeT RequirementID = TConfig::template GetRequirementID<TRequirement>();

			return std::get<RequirementID>(BitArrayStorage);
		}

	private:
		TBitArrayTuple BitArrayStorage;

		//Set the Requirement BitArray based on Component and Tag Types
		template<typename TRequirement>
		void InitializeBitArray()
		{
			TRequirementBitArray& BitArray = GetRequirementBitArray<TRequirement>();
			
			using ComponentsInRequirement = TComponentsInRequirement<TRequirement>;

			ForTypes<ComponentsInRequirement>
				([this, &BitArray](auto Component)
				{
					//std::cout << "Component: "<<typeid(Component).name() << std::endl;

					using ComponentType = typename decltype(Component)::Type;

					constexpr SizeT ComponentBit = TConfig::template GetComponentBit<ComponentType>();

					BitArray[ComponentBit] = true;
				});

			using TagsInRequirement = TTagsInRequirement<TRequirement>;

			ForTypes<TagsInRequirement>
				([this, &BitArray](auto Tag)
				{
					//std::cout <<"Tag: "<< typeid(Tag).name() << std::endl;

					using TagType = typename decltype(Tag)::Type;

					constexpr SizeT TagBit = TConfig::template GetTagBit<TagType>();

					BitArray[TagBit] = true;
				});
		}

	};
}

#endif
