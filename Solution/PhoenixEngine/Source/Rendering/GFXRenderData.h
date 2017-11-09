#ifndef PHOENIX_GFX_RENDER_DATA_H
#define PHOENIX_GFX_RENDER_DATA_H

#include "Utility/Containers/Vector.h"
#include "Utility/Misc/Allocator.h"
#include "Rendering/Camera.h"
#include "Rendering/Image/ImageInstance.h"
#include "Rendering/Model/ModelInstance.h"

namespace Phoenix
{
	struct FGFXRenderData
	{
		F_RequireClassHeapAlignment(16);
	public:
		FCamera PersCam;
		FCamera OrthoCam;
		TVector<FImageInstance> Images;
		TVector<FModelInstance> Models;
		bool IsReadyForRender{ false };
	};
}

#endif
