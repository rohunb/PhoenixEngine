#ifndef PHOENIX_RENAME_H
#define PHOENIX_RENAME_H

namespace Phoenix
{
	template<typename TOldTypeContainer, template<typename...> class TNewTypeContainer>
	struct TRenameImpl;

	template<template<typename...> class TOldTypeContainer
			, template<typename...> class TNewTypeContainer
			, typename... Ts
			>
	struct TRenameImpl<TOldTypeContainer<Ts...>, TNewTypeContainer>
	{
		using Type = TNewTypeContainer<Ts...>;
	};

	/*! \brief Changes the type of a TypeContainer
	*
	*	@example TRename<TTypeList<Comp1, Comp2>, TTuple>
	*			 Results in: TTuple<Comp1, Comp2>
	*/
	template<typename TOldTypeContainer
			, template<typename...> class TNewTypeContainer
			>
	using TRename = typename TRenameImpl<TOldTypeContainer, TNewTypeContainer>::Type;
}


#endif
