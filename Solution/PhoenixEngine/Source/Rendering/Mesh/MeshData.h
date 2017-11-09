#pragma once

#include "ExternalLib/GLIncludes.h"
#include "Utility/Containers/Array.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Debug/Assert.h"
#include "Math/Vector2D.h"
#include "Rendering/Image/ImageData.h"

namespace Phoenix
{
	namespace EMeshAttribute
	{
		typedef UInt16 Type;

		enum BitMask : Type
		{
			None = 0,
			Positions = 1 << 0,
			Normals = 1 << 1,
			Indices = 1 << 2,
			UVCoords = 1 << 3,
			DiffuseMap = 1 << 4,
			NormalMap = 1 << 5,
			SpecularMap = 1 << 6,
			Material = 1 << 7,
			All = Positions | Normals | Indices | UVCoords | DiffuseMap | NormalMap | SpecularMap | Material
		};
	}

	namespace EMeshDataIndex
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			Diffuse,
			Normal,
			Count
		};
	}

	namespace EMeshPositionType
	{
		typedef UInt8 Type;
		typedef GLsizei SizeType;

		enum Value : Type
		{
			None,
			XY = 2,
			XYZ = 3,
		};

		static inline SizeType GetSize(const EMeshPositionType::Value PositionType)
		{
			F_Assert(PositionType != EMeshPositionType::None, "Invalid position type.");
			const SizeType Result = PositionType;
			return Result;
		}
	}

	struct FMeshData
	{
		typedef GLfloat PositionT;
		typedef GLfloat NormalT;
		typedef GLfloat UVCoordT;
		typedef UInt8 IndexT;
		typedef UInt8 IndexTSizeT;
		typedef GLsizei VertexCountT;
		typedef TVector<FMeshData> FEntries;
		typedef UInt16 TexNameIndexT;

		EMeshAttribute::Type MeshAttrib{ EMeshAttribute::None };
		TVector<PositionT> Positions;
		TVector<NormalT> Normals;
		TVector<UVCoordT> UVCoords;
		TVector<IndexT> Indices;
		IndexTSizeT IndexTSize{ 0 };
		EMeshPositionType::Value PositionType{ EMeshPositionType::None };
		VertexCountT VertexCount{ 0 };
		TVector<FChar> TextureNames;
		TArray<TexNameIndexT, EMeshDataIndex::Count> TexNameIndices;

		FMeshData()
		{
			TexNameIndices.fill(0);
		}

		bool HasTextureName(const EMeshDataIndex::Value Index) const
		{
			const bool Result = TexNameIndices[Index] != 0;
			return Result;
		}

		const FChar* const GetTextureName(const EMeshDataIndex::Value Index) const
		{
			const TexNameIndexT TexNameIndex = TexNameIndices[Index];
			const FChar* const TextureName = &TextureNames[TexNameIndex];
			return TextureName;
		}

		static FMeshData CreateQuadMesh()
		{
			const FVector2D BottomLeft(-1, -1);
			const FVector2D BottomRight(1, -1);
			const FVector2D TopLeft(-1, 1);
			const FVector2D TopRight(1, 1);

			return CreateQuadMesh(BottomLeft, BottomRight, TopLeft, TopRight);
		}

		static FMeshData CreateQuadMesh(
			const FVector2D BottomLeft,
			const FVector2D BottomRight,
			const FVector2D TopLeft,
			const FVector2D TopRight)
		{
			FMeshData MeshData;

			MeshData.MeshAttrib = EMeshAttribute::Positions | EMeshAttribute::UVCoords;
			MeshData.Positions =
			{
				BottomLeft.x, BottomLeft.y,
				BottomRight.x, BottomRight.y,
				TopRight.x, TopRight.y,

				BottomLeft.x, BottomLeft.y,
				TopRight.x, TopRight.y,
				TopLeft.x, TopLeft.y
			};

			MeshData.UVCoords =
			{
				0, 0,
				1, 0,
				1, 1,

				0, 0,
				1, 1,
				0, 1
			};

			MeshData.PositionType = EMeshPositionType::XY;
			MeshData.VertexCount = 6;
			MeshData.TextureNames = { '\0' };

			return MeshData;
		}

		template<typename TSerializer>
		void Serialize(TSerializer& Serializer)
		{
			Serializer.Serialize(
				MeshAttrib, Positions, Normals, UVCoords, Indices, 
				IndexTSize, PositionType, VertexCount, TextureNames, TexNameIndices);
		}

		//Debug
		bool operator==(const FMeshData& RHS) const
		{
			if (this == &RHS)
			{
				return true;
			}

			const bool MeshAttribSame = MeshAttrib == RHS.MeshAttrib;
			const bool PositionsSame = Positions == RHS.Positions;
			const bool NormalsSame = Normals == RHS.Normals;
			const bool UVSame = UVCoords == RHS.UVCoords;
			const bool IndicesSame = Indices == RHS.Indices;
			const bool IndexSizeSame = IndexTSize == RHS.IndexTSize;
			const bool PosTypeSame = PositionType == RHS.PositionType;
			const bool VertexCountSame = VertexCount == RHS.VertexCount;
			const bool TextureNamesSame = TextureNames == RHS.TextureNames;
			const bool TexNameIndicesSame = TexNameIndices == RHS.TexNameIndices;

			const bool Match = 
				MeshAttribSame && PositionsSame && NormalsSame && UVSame && IndicesSame && 
				IndexSizeSame && PosTypeSame && VertexCountSame && TextureNamesSame && TexNameIndicesSame;

			return Match;
		}
	};
}
