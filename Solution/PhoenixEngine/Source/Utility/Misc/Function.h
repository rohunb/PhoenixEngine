#ifndef PHOENIX_FUNCTION_H
#define PHOENIX_FUNCTION_H

#include <functional>

namespace Phoenix
{
	template<class T>
	using TFunction = std::function<T>;
}

#endif
