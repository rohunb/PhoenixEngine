#ifndef PHOENIX_TYPE_WRAPPER_H
#define PHOENIX_TYPE_WRAPPER_H

namespace Phoenix
{
	/*! \brief A Wrapper to contain a type.
		\ Primary use is to instantiate and get an object's type without actually instigating the underlying type
	*/
	template<typename T>
	struct TTypeWrapper
	{
		using Type = T;
	};
}
#endif
