#include "Stdafx.h"
#include "Rendering/Mesh/Mesh.h"

#include "Utility/Debug/Assert.h"
#include "Rendering/Caches/ImageCache.h"
#include "Rendering/Debug/GFXDebug.h"

using namespace Phoenix;
using namespace Phoenix::GL;

FMesh::FMesh(FMesh&& RHS)
	: VertexArray(RHS.VertexArray)
	, VertexBuffer(RHS.VertexBuffer)
	, ElementBuffer(RHS.ElementBuffer)
	, IndexCount(RHS.IndexCount)
	, IndexTSize(RHS.IndexTSize)
	, VertexCount(RHS.VertexCount)
	, DiffuseImage(std::move(RHS.DiffuseImage))
	, NormalImage(std::move(RHS.NormalImage))
{
	RHS.PostMoveReset();
}

FMesh& FMesh::operator=(FMesh&& RHS)
{
	if (this == &RHS)
	{
		return *this;
	}

	DeInit();

	VertexArray = RHS.VertexArray;
	VertexBuffer = RHS.VertexBuffer;
	ElementBuffer = RHS.ElementBuffer;
	IndexCount = RHS.IndexCount;
	IndexTSize = RHS.IndexTSize;
	VertexCount = RHS.VertexCount;
	DiffuseImage = std::move(RHS.DiffuseImage);
	NormalImage = std::move(RHS.NormalImage);

	RHS.PostMoveReset();
	return *this;
}

FMesh::~FMesh()
{
	DeInit();
}

void FMesh::Init(const FMeshData& MeshData, const FImageCache& ImageCache)
{
	F_Assert(MeshData.Positions.size(), "Position data is missing.");

	const bool bNormalsProvided = !MeshData.Normals.empty();
	const bool bUVCoordsProvided = !MeshData.UVCoords.empty();
	const bool bIndicesProvided = !MeshData.Indices.empty();
	
	DeInit();

	F_GL(GL::GenVertexArrays(1, &VertexArray));
	F_GL(GL::GenBuffers(1, &VertexBuffer));

	F_GL(GL::BindVertexArray(VertexArray));

	{
		const GLsizeiptr PositionsSize = MeshData.Positions.size() * sizeof(FMeshData::PositionT);
		const GLsizeiptr NormalsSize = MeshData.Normals.size() * sizeof(FMeshData::NormalT);
		const GLsizeiptr UVCoordsSize = MeshData.UVCoords.size() * sizeof(FMeshData::UVCoordT);

		const GLsizeiptr TotalSize = PositionsSize + NormalsSize + UVCoordsSize;

		const GLsizeiptr NormalsStart = PositionsSize;
		const GLsizeiptr UVCoordsStart = NormalsStart + NormalsSize;

		const GLvoid* const NormalsStartPtr = reinterpret_cast<GLvoid*>(NormalsStart);
		const GLvoid* const UVCoordsStartPtr = reinterpret_cast<GLvoid*>(UVCoordsStart);

		F_GL(GL::BindBuffer(EBuffer::Array, VertexArray));
		F_GL(GL::BufferData(EBuffer::Array, TotalSize, nullptr, EUsage::StaticDraw));

		GLuint CurrentIndex = 0;

		const EMeshPositionType::SizeType Size = EMeshPositionType::GetSize(MeshData.PositionType);

		F_GL(GL::BufferSubData(EBuffer::Array, 0, PositionsSize, &MeshData.Positions[0]));
		F_GL(GL::EnableVertexAttribArray(CurrentIndex));
		F_GL(GL::VertexAttribPointer(CurrentIndex, Size, EType::Float, EBool::False, Size * sizeof(GLfloat), nullptr));
		
		if (bNormalsProvided)
		{
			++CurrentIndex;
			F_GL(GL::BufferSubData(EBuffer::Array, NormalsStart, NormalsSize, &MeshData.Normals[0]));
			F_GL(GL::EnableVertexAttribArray(CurrentIndex));
			F_GL(GL::VertexAttribPointer(CurrentIndex, 3, EType::Float, EBool::False, 3 * sizeof(GLfloat), NormalsStartPtr));
		}

		if (bUVCoordsProvided)
		{
			++CurrentIndex;
			F_GL(GL::BufferSubData(EBuffer::Array, UVCoordsStart, UVCoordsSize, &MeshData.UVCoords[0]));
			F_GL(GL::EnableVertexAttribArray(CurrentIndex));
			F_GL(GL::VertexAttribPointer(CurrentIndex, 2, EType::Float, EBool::False, 2 * sizeof(GLfloat), UVCoordsStartPtr));
		}
	}

	if (bIndicesProvided)
	{
		const GLsizeiptr Size = MeshData.Indices.size() * sizeof(FMeshData::IndexT);

		F_GL(GL::GenBuffers(1, &ElementBuffer));
		F_GL(GL::BindBuffer(EBuffer::ElementArray, ElementBuffer));
		F_GL(GL::BufferData(EBuffer::ElementArray, Size, &MeshData.Indices[0], EUsage::StaticDraw));

		IndexCount = Size / MeshData.IndexTSize;
		IndexTSize = MeshData.IndexTSize;
	}

	VertexCount = MeshData.VertexCount;

	F_GL(GL::BindVertexArray(0));

	if (MeshData.HasTextureName(EMeshDataIndex::Diffuse))
	{
		const FChar* const DiffuseName = MeshData.GetTextureName(EMeshDataIndex::Diffuse);
		DiffuseImage = ImageCache.GetItem(DiffuseName, DiffuseImage);
	}
	
	F_GFXLog("Mesh VAO Created: " << VertexArray << " " << VertexBuffer << " " << ElementBuffer);
}

bool FMesh::IsValid() const
{
	const bool Result = VertexArray != 0;
	return Result;
}

void FMesh::DeInit()
{
	if (VertexArray)
	{
		F_GFXLog("Mesh VAO Destroyed: " << VertexArray << " " << VertexBuffer << " " << ElementBuffer);
		F_GL(GL::DeleteVertexArrays(1, &VertexArray));
		VertexArray = 0;
	}

	if (VertexBuffer)
	{
		F_GL(GL::DeleteBuffers(1, &VertexBuffer));
		VertexBuffer = 0;
	}

	if (ElementBuffer)
	{
		F_GL(GL::DeleteBuffers(1, &ElementBuffer));
		ElementBuffer = 0;
	}

	IndexCount = 0;
	IndexTSize = 0;
	VertexCount = 0;
	DiffuseImage.DeInit();
	NormalImage.DeInit();
}

GL::VertexArrayT FMesh::GetVertexArray() const
{
	return VertexArray;
}

GL::ElementBufferT FMesh::GetElementBuffer() const
{
	return ElementBuffer;
}

GL::IndexCountT FMesh::GetIndexCount() const
{
	return IndexCount;
}

FMeshData::IndexTSizeT FMesh::GetIndexTypeSize() const
{
	return IndexTSize;
}

EType::Value FMesh::GetIndexType() const
{
	const UInt8 UInt8Size = sizeof(UInt8);
	const UInt8 UInt16Size = sizeof(UInt16);
	const UInt8 UInt32Size = sizeof(UInt32);

	switch (IndexTSize)
	{
		case UInt8Size:
		{
			return EType::UByte;
		}

		case UInt16Size:
		{
			return EType::UShort;
		}

		case UInt32Size:
		{
			return EType::UInt;
		}

		default:
		{
			F_Assert(false, "Invalid size of " << IndexTSize);
			break;
		}
	}

	F_Assert(false, "Invalid size of " << IndexTSize);
	return EType::UByte;
}

FMeshData::VertexCountT FMesh::GetVertexCount() const
{
	return VertexCount;
}

EMeshAttribute::Type FMesh::GetValidImages() const
{
	EMeshAttribute::Type MeshAttrib = EMeshAttribute::None;
	
	if (DiffuseImage.IsValid())
	{
		MeshAttrib |= EMeshAttribute::DiffuseMap;
	}

	return MeshAttrib;
}

const THandle<FImage>& FMesh::GetDiffuseImage() const
{
	return DiffuseImage;
}

void FMesh::PostMoveReset()
{
	VertexArray = 0;
	VertexBuffer = 0;
	ElementBuffer = 0;
	IndexCount = 0;
	IndexTSize = 0;
	VertexCount = 0;
	DiffuseImage.DeInit();
	NormalImage.DeInit();
}
