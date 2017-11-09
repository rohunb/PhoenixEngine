#ifndef PHOENIX_FONT_CACHE_H
#define PHOENIX_FONT_CACHE_H

#include "Utility/Containers/NamedItemCache.h"
#include "Utility/Handle/Handle.h"
#include "Rendering/Text/Font.h"

namespace Phoenix
{
	class FFontCache : public TNamedItemCache<THandle<FFont>> {};
}

#endif
