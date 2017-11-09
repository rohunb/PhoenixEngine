#include "Stdafx.h"
#include "Rendering/Model/Model.h"

#include "Utility/Debug/Assert.h"
#include "Rendering/Caches/ImageCache.h"
#include "Rendering/Debug/GFXProfiler.h"
#include "Rendering/GL/GLInterface.h"

using namespace Phoenix;

void FModel::Init(const FMeshData::FEntries& MeshDataEntries, const FImageCache& ImageCache)
{
	F_GFXProfile();
	F_Assert(MeshDataEntries.size(), "Invalid mesh data entries size of 0.");
	DeInit();

	Meshes.resize(MeshDataEntries.size());

	const SizeT Size = Meshes.size();

	for (SizeT I = 0; I < Size; ++I)
	{
		const FMeshData& MeshData = MeshDataEntries[I];
		Meshes[I].Init(MeshData, ImageCache);
	}
}

bool FModel::IsValid() const
{
	for (const auto& Mesh : Meshes)
	{
		if (!Mesh.IsValid())
		{
			return false;
		}
	}

	return true;
}

void FModel::DeInit()
{
	Meshes.clear();
}

const FModel::FMeshes& FModel::GetMeshes() const
{
	return Meshes;
}
