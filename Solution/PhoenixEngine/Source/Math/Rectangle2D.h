#ifndef PHOENIX_RECTANGLE_2D_H
#define PHOENIX_RECTANGLE_2D_H

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	template <class T>
	struct TRectangle2D
	{
		T X{ T(0) };
		T Y{ T(0) };

		T Width{ T(0) };
		T Height{ T(0) };
	};

	typedef TRectangle2D<Float32> FRectangle2D;
	typedef TRectangle2D<Int32> FRectangle2Di;
}

#endif
