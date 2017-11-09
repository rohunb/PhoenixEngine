#ifndef PHOENIX_MODEL_CACHE_H
#define PHOENIX_MODEL_CACHE_H

#include "Utility/Containers/NamedItemCache.h"
#include "Utility/Handle/Handle.h"
#include "Rendering/Model/Model.h"

namespace Phoenix
{
	class FModelCache : public TNamedItemCache<THandle<FModel>> {};
}

#endif
