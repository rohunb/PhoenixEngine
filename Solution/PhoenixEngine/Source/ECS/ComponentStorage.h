#ifndef PHOENIX_COMPONENT_STORAGE
#define PHOENIX_COMPONENT_STORAGE

#include "Utility/Containers/Tuple.h"
#include "Utility/Containers/Vector.h"
#include "Utility/MetaProgramming/For.h"
#include "Utility/MetaProgramming/Rename.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	template<typename TConfig>
	class TComponentStorage
	{
	public:
		/*! \brief Resize all the component vectors
		*/
		void Resize(SizeT NewCapacity);

		template<typename TComponent>
		TVector<TComponent>& GetComponentVector();

		template<typename TComponent>
		TComponent& GetComponent(SizeT Index);

		template<typename TComponent>
		const TComponent& GetComponent(SizeT Index) const;

		void Swap(SizeT LeftIndex, SizeT RightIndex)
		{
			F_Assert(LeftIndex < RightIndex, "Left Index should be less than RightIndex");
			F_Assert(RightIndex <= std::get<0>(TupleOfComponentVectors).size(), "RightIndex is past Component Vector Size");

			ForTuple(TupleOfComponentVectors
				, [LeftIndex, RightIndex](auto& ComponentVector) 
				{
					std::swap(ComponentVector[LeftIndex], ComponentVector[RightIndex]);
				});
		}

	private:
		using ComponentList = typename TConfig::ComponentList;

		template<typename... Ts>
		using TupleOfVectors = TTuple<TVector<Ts>...>;

		using TTupleOfComponentVectors = TRename<ComponentList, TupleOfVectors>;

		TTupleOfComponentVectors TupleOfComponentVectors;
	};

	template<typename TConfig>
	void TComponentStorage<TConfig>::Resize(SizeT NewCapacity)
	{
		ForTuple(TupleOfComponentVectors
				, [NewCapacity](auto& ComponentVector)
				  {
					  ComponentVector.resize(NewCapacity);
				  });
	}

	template<typename TConfig>
	template<typename TComponent>
	TVector<TComponent>& TComponentStorage<TConfig>::GetComponentVector()
	{
		return std::get<TVector<TComponent>>(TupleOfComponentVectors);
	}

	template<typename TConfig>
	template<typename TComponent>
	TComponent& TComponentStorage<TConfig>::GetComponent(SizeT Index)
	{
		static_assert(TConfig::template IsComponent<TComponent>(), "Not a component");

		auto& ComponentVector = GetComponentVector<TComponent>();
		return ComponentVector[Index];
	}

	template<typename TConfig>
	template<typename TComponent>
	const TComponent& TComponentStorage<TConfig>::GetComponent(SizeT Index) const
	{
		static_assert(TConfig::template IsComponent<TComponent>(), "Not a component");

		const auto& ComponentVector = GetComponentVector<TComponent>();
		return ComponentVector[Index];
	}
}

#endif
