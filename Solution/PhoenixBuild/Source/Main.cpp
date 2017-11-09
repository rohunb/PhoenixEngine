#include "Utility/Containers/Vector.h"
#include "Utility/Debug/Debug.h"
#include "Utility/Debug/Profiler.h"
#include "Utility/Misc/StringStream.h"
#include "Rendering/GFXUtils.h"

#include "ModelConverter/ModelConverter.h"

int main()
{
	using namespace Phoenix;
	F_LogOpen("BuildLog.txt");

	{
		F_Profile();

		TVector<const FChar*> Models =
		{
			"golem"
		};

		TVector<const FChar*> ModelSuffixes =
		{
			".fbx"
		};

		TVector<FVector3D> Offsets =
		{
			FVector3D(0, 50, 0),
		};

		TVector<Float32> Sizes =
		{
			1.f,
		};

		const FChar* const ModelsPath = EAssetPath::Get(EAssetPath::Models);
		for (SizeT I = 0, Max = Models.size(); I < Max; ++I)
		{
			FStringStream SS;
			SS << ModelsPath << Models[I];

			const FString ModelLoadPath = SS.str() + ModelSuffixes[I];
			const FString ModelSavePath = SS.str() + ".pmesh";

			FModelConverter::ConvertModelToBinary(ModelLoadPath.c_str(), ModelSavePath.c_str(), Offsets[I], Sizes[I]);
		}
	}

	F_Log(F_GetProfiler());
	F_LogClose();
	getchar();
	return 0;
}
