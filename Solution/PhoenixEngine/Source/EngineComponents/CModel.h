#ifndef PHOENIX_C_MODEL_H
#define PHOENIX_C_MODEL_H

#include "Rendering/Model/ModelInstance.h"
#include "Utility/Handle/Handle.h"
#include "Utility/Misc/String.h"
#include "Utility/Debug/Assert.h"

namespace Phoenix
{
	struct CModel
	{
		FString ModelFileName;
		THandle<FModelInstance> ModelInstance;

		CModel() = default;

		explicit CModel(const FString& ModelFileName)
			: ModelFileName(ModelFileName)
		{
			F_Assert(!ModelFileName.empty(), "Model Name is Empty");
		}

		
	};
}

#endif
