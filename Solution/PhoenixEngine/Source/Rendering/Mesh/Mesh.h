#ifndef PHOENIX_MESH_H
#define PHOENIX_MESH_H

#include "ExternalLib/GLIncludes.h"
#include "Utility/Handle/Handle.h"
#include "Utility/Misc/Primitives.h"
#include "Rendering/GL/GLInterface.h"
#include "Rendering/Image/Image.h"
#include "Rendering/Mesh/MeshData.h"

namespace Phoenix
{
	class FMesh
	{
	public:
		FMesh() = default;

		FMesh(const FMesh&) = delete;
		FMesh& operator=(const FMesh&) = delete;

		FMesh(FMesh&&);
		FMesh& operator=(FMesh&&);

		~FMesh();

		void Init(const FMeshData& MeshData, const class FImageCache& ImageCache);

		bool IsValid() const;

		void DeInit();

		GL::VertexArrayT GetVertexArray() const;
		
		GL::ElementBufferT GetElementBuffer() const;

		GL::IndexCountT GetIndexCount() const;

		FMeshData::IndexTSizeT GetIndexTypeSize() const;

		GL::EType::Value GetIndexType() const;

		FMeshData::VertexCountT GetVertexCount() const;

		EMeshAttribute::Type GetValidImages() const;

		const THandle<FImage>& GetDiffuseImage() const;
		
	private:
		GL::VertexArrayT VertexArray{ 0 };
		GL::VertexBufferT VertexBuffer{ 0 };
		GL::ElementBufferT ElementBuffer{ 0 };
		GL::IndexCountT IndexCount{ 0 };
		FMeshData::IndexTSizeT IndexTSize{ 0 };
		FMeshData::VertexCountT VertexCount{ 0 };
		THandle<FImage> DiffuseImage;
		THandle<FImage> NormalImage;

		void PostMoveReset();
	};
}

#endif
