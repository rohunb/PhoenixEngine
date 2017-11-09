#ifndef PHOENIX_ENTITY_H
#define PHOENIX_ENTITY_H

namespace Phoenix
{
	template<typename TComponentsBitArray>
	struct TEntity
	{
		using ComponentsBitArray = TComponentsBitArray;

		//Index into the Array of Components
		SizeT ComponentArrayIndex { 0 };

		//Indicates which Components and Tags the Entity has
		ComponentsBitArray BitArray;

		bool Alive { false };

		void Reset(SizeT Index);
	};

	template<typename TConfig>
	void TEntity<TConfig>::Reset(SizeT Index)
	{
		ComponentArrayIndex = Index;
		Alive = false;
		BitArray.reset();
	}
}

#endif
