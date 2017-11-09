#include "Stdafx.h"
#include "Rendering/Model/ModelProcessor.h"

#include "ExternalLib/AssimpIncludes.h"
#include "ExternalLib/FBXIncludes.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Debug/Assert.h"
#include "Utility/FileIO/Endian.h"
#include "Utility/FileIO/File.h"
#include "Utility/FileIO/FileStream.h"
#include "Utility/Misc/Memory.h"
#include "Utility/Misc/String.h"
#include "Utility/Serialization/BinarySerializer.h"
#include "Utility/Serialization/BinaryDeserializer.h"
#include "Math/Math.h"
#include "Math/MathCommon.h"
#include "Rendering/Debug/GFXDebug.h"
#include "Rendering/Mesh/MeshData.h"

namespace Phoenix
{
	struct FModelProcessorHelper
	{
		static const UInt8 FloatsPerVertex;
		static const UInt8 FloatsPerUVCoord;
		static const UInt8 IndicesPerFace;
		static const UInt8 UVCoordsPerTriangle;
		static const UInt8 VerticesPerTriangle;

		static const FChar* const GetAttributeTypeName(const FbxNodeAttribute::EType Type);

		static bool IsFileFBX(const FChar* const File);

		static bool IsFileMisc(const FChar* const File);

		static bool IsFilePhoenix(const FChar* const File);

		static void ProcessMesh(
			FMeshData& MeshData,
			const aiScene& AIScene,
			const aiMesh& AIMesh);

		template <class IndexT>
		static void ProcessMeshFaces(
			FMeshData& MeshData,
			const aiScene& AIScene,
			const aiMesh& AIMesh);

		static void ProcessMaterial(
			FMeshData& MeshData,
			FMeshData::TexNameIndexT& TextureNameIndex,
			const aiScene& AIScene,
			const aiMesh& AIMesh,
			const aiTextureType AITextureType);

		static void ProcessMesh(
			FMeshData& MeshData,
			const fbxsdk::FbxNode& FBXNode,
			fbxsdk::FbxMesh& FBXMesh);

		static void ProcessMaterial(
			FMeshData& MeshData,
			FMeshData::TexNameIndexT& TextureNameIndex,
			const fbxsdk::FbxSurfaceMaterial& FBXSurfaceMaterial,
			const FChar* const PropertyName);

		static void AppendTextureName(
			FMeshData& MeshData,
			FMeshData::TexNameIndexT& TextureNameIndex,
			const FChar* const Str,
			const SizeT StrLength);

		static void OnMeshEntriesLoaded(FMeshData::FEntries& MeshEntries, const FModelProcessor::FLoadParams& LoadParams);
		static void ProcessOriginOffset(FMeshData::FEntries& MeshEntries, const FVector3D& OriginOffset);
		static void ProcessSize(FMeshData::FEntries& MeshEntries, const Float32 NormalizedSize);
	};
}

using namespace Phoenix;

const SizeT FModelProcessor::DefaultNodeCapacity = 32;
const SizeT FModelProcessor::DefaultMeshEntriesCapacity = 16;

const UInt8 FModelProcessorHelper::FloatsPerVertex = 3;
const UInt8 FModelProcessorHelper::FloatsPerUVCoord = 2;
const UInt8 FModelProcessorHelper::IndicesPerFace = 3;
const UInt8 FModelProcessorHelper::UVCoordsPerTriangle = 3;
const UInt8 FModelProcessorHelper::VerticesPerTriangle = 3;

FModelProcessor::FLoadParams::FLoadParams(const FChar* InFile, const EMeshAttribute::Type InMeshAttributeHints)
	: File(InFile)
	, MeshAttributeHints(InMeshAttributeHints)
{
}

void FModelProcessor::Load(const FLoadParams& LoadParams)
{
	F_Assert(LoadParams.File, "No file was specified.");
	F_Assert(LoadParams.MeshAttributeHints, "No hints were specified.");

	Unload();

	FLoadFunction const LoadFunction = GetLoadFunction(LoadParams);

	if (!LoadFunction)
	{
		F_GFXLogError("Failed to get load function for file " << LoadParams.File);
		return;
	}

	(this->*LoadFunction)(LoadParams);
}

bool FModelProcessor::IsValid() const
{
	const bool Result = MeshEntries.size() != 0;
	return Result;
}

void FModelProcessor::Save(const FSaveParams& SaveParams)
{
	{
		FOutputFileStream OutStream{ SaveParams.File, FFile::EFileOpenMode::Binary };
		FBinarySerializer Serializer{ OutStream };
		Serializer.Serialize(MeshEntries);
	}
	{
		//Debug
		FMeshData::FEntries InMeshEntries;
		FInputFileStream InStream{ SaveParams.File , FFile::EFileOpenMode::Binary };
		FBinaryDeserializer Deserializer{ InStream };
		Deserializer.Serialize(InMeshEntries);

		F_AssertEqual(MeshEntries, InMeshEntries, "serialize failed");
	}
}

void FModelProcessor::Unload()
{
	MeshEntries.clear();
}

const FMeshData::FEntries& FModelProcessor::GetMeshData() const
{
	return MeshEntries;
}

FModelProcessor::FLoadFunction FModelProcessor::GetLoadFunction(const FLoadParams& LoadParams)
{
	if (FModelProcessorHelper::IsFilePhoenix(LoadParams.File))
	{
		return &FModelProcessor::LoadPhoenix;
	}

	if (FModelProcessorHelper::IsFileFBX(LoadParams.File))
	{
		return &FModelProcessor::LoadFBX;
	}

	if (FModelProcessorHelper::IsFileMisc(LoadParams.File))
	{
		return &FModelProcessor::LoadMisc;
	}

	F_GFXLogError("Could not retrieve load function for { " << LoadParams.File << " }");
	return nullptr;
}

void FModelProcessor::LoadFBX(const FLoadParams& LoadParams)
{
	TRawPtr<FbxManager> FBXManager = FbxManager::Create();

	if (!FBXManager.IsValid())
	{
		F_GFXLogError("FBX SDK is an evaluation copy and it has expired.  See FbxManager::Create().");
		return;
	}

	TRawPtr<FbxIOSettings> IOSettings = FbxIOSettings::Create(FBXManager.Get(), IOSROOT);
	IOSettings->SetBoolProp(IMP_FBX_MATERIAL, true);
	IOSettings->SetBoolProp(IMP_FBX_TEXTURE, true);
	IOSettings->SetBoolProp(IMP_FBX_LINK, false);
	IOSettings->SetBoolProp(IMP_FBX_SHAPE, false);
	IOSettings->SetBoolProp(IMP_FBX_GOBO, false);
	IOSettings->SetBoolProp(IMP_FBX_ANIMATION, true);
	IOSettings->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	FBXManager->SetIOSettings(IOSettings.Get());

	TRawPtr<FbxImporter> FBXImporter = FbxImporter::Create(FBXManager.Get(), "Importer");

	if (!FBXImporter->Initialize(LoadParams.File, -1, FBXManager->GetIOSettings()))
	{
		F_GFXLogError("Failed to load fbx model.  Error log: " << FBXImporter->GetStatus().GetErrorString());
		FBXManager->Destroy();
		FBXManager = nullptr;
		return;
	}

	TRawPtr<FbxScene> FBXScene = FbxScene::Create(FBXManager.Get(), "Scene");
	FBXImporter->Import(FBXScene.Get());
	FBXImporter->Destroy();
	FBXImporter = nullptr;

	TRawPtr<FbxNode> RootFBXNode = FBXScene->GetRootNode();
	TVector<TRawPtr<FbxNode>> FBXNodes;

	FBXNodes.reserve(FModelProcessor::DefaultNodeCapacity);

	if (RootFBXNode.IsValid())
	{
		FBXNodes.push_back(RootFBXNode.Get());
	}

	for (SizeT I = 0; I < FBXNodes.size(); ++I)
	{
		for (Int32 ChildI = 0; ChildI < FBXNodes[I]->GetChildCount(); ++ChildI)
		{
			F_Assert(FBXNodes[I]->GetChild(ChildI),
				"Node index " << I << " has null node at index " << ChildI << ".");

			FbxNode* const ChildNode = FBXNodes[I]->GetChild(ChildI);
			FBXNodes.push_back(ChildNode);
		}
	}

	F_Assert(MeshEntries.size() == 0, "Mesh entries were not cleared prior to this operation.");
	MeshEntries.reserve(FModelProcessor::DefaultMeshEntriesCapacity);
	FbxGeometryConverter GeometryConverter(FBXManager.Get());

	for (SizeT I = 0; I < FBXNodes.size(); ++I)
	{
		FbxNode& FBXNode = *FBXNodes[I];

		for (Int32 AttribI = 0; AttribI < FBXNode.GetNodeAttributeCount(); ++AttribI)
		{
			TRawPtr<FbxNodeAttribute> FBXNodeAttribute = FBXNode.GetNodeAttributeByIndex(AttribI);
			const FbxNodeAttribute::EType NodeAttributeType = FBXNodeAttribute->GetAttributeType();

			switch (NodeAttributeType)
			{
				case FbxNodeAttribute::eMesh:
				{
					{
						FMeshData MeshData;
						MeshData.MeshAttrib = LoadParams.MeshAttributeHints;

						MeshEntries.emplace_back(std::move(MeshData));
					}

					const SizeT MeshEntryIndex = MeshEntries.size() - 1;
					FMeshData& MeshData = MeshEntries[MeshEntryIndex];

					TRawPtr<FbxMesh> FBXMesh = static_cast<FbxMesh*>(FBXNodeAttribute.Get());

					if (!FBXMesh->IsTriangleMesh())
					{
						const bool ReplaceOriginalGeometry = true;
						FBXNodeAttribute = GeometryConverter.Triangulate(FBXMesh.Get(), ReplaceOriginalGeometry);
						FBXMesh = static_cast<FbxMesh*>(FBXNodeAttribute.Get());
					}

					FModelProcessorHelper::ProcessMesh(MeshData, FBXNode, *FBXMesh);

					if (MeshData.MeshAttrib == EMeshAttribute::None)
					{
						F_GFXLogError("Mesh entry initialization failed.");
						MeshEntries.pop_back();
					}
					break;
				}

				default:
				{
					// Note: Do nothing for now.
					break;
				}
			}
		}
	}

	FBXManager->Destroy();
	FBXManager = nullptr;

	FModelProcessorHelper::OnMeshEntriesLoaded(MeshEntries, LoadParams);
}

void FModelProcessor::LoadMisc(const FLoadParams& LoadParams)
{
	F_Assert(LoadParams.File, "File was not specified.");
	F_Assert(LoadParams.MeshAttributeHints, "Mesh attribute hints were not specified.");

	const UInt32 Flags =
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_ValidateDataStructure |
		aiProcess_ImproveCacheLocality |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph |
		aiProcess_FlipUVs;

	Assimp::Importer ModelLoader;
	const aiScene* const AIScene = ModelLoader.ReadFile(LoadParams.File, Flags);

	const bool SceneIsValid = AIScene != nullptr;

	if (!SceneIsValid)
	{
		F_GFXLogError("Failed to load misc model.  Error log: " << ModelLoader.GetErrorString());
		return;
	}

	F_Assert(AIScene->mRootNode != nullptr, "Root node is invalid.");
	F_Assert((AIScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == 0, "Scene flags are invalid.");

	const aiScene& AISceneRef = *AIScene;

	TVector<const aiNode*> AINodes;

	AINodes.reserve(FModelProcessor::DefaultNodeCapacity);
	AINodes.push_back(AISceneRef.mRootNode);

	for (SizeT I = 0; I < AINodes.size(); ++I)
	{
		for (UInt32 ChildI = 0; ChildI < AINodes[I]->mNumChildren; ++ChildI)
		{
			F_Assert(AINodes[I]->mChildren[ChildI],
				"Node index " << I << " has null mesh at index " << ChildI << ".");

			const aiNode* const ChildNode = AINodes[I]->mChildren[ChildI];
			AINodes.push_back(ChildNode);
		}
	}

	F_Assert(MeshEntries.size() == 0, "Mesh entries were not cleared prior to this operation.");
	MeshEntries.reserve(FModelProcessor::DefaultMeshEntriesCapacity);

	const SizeT AINodesSize = AINodes.size();
	for (SizeT I = 0; I < AINodesSize; ++I)
	{
		for (UInt32 MeshI = 0; MeshI < AINodes[I]->mNumMeshes; ++MeshI)
		{
			{
				FMeshData MeshData;
				MeshData.MeshAttrib = LoadParams.MeshAttributeHints;

				MeshEntries.emplace_back(std::move(MeshData));
			}

			{
				const UInt32 Handle = AINodes[I]->mMeshes[MeshI];
				F_Assert(Handle < AISceneRef.mNumMeshes,
					"Index " << Handle << " is out of range of " << AISceneRef.mNumMeshes);

				const SizeT MeshEntryIndex = MeshEntries.size() - 1;
				FMeshData& MeshData = MeshEntries[MeshEntryIndex];

				const aiMesh& AIMesh = *AISceneRef.mMeshes[Handle];
				FModelProcessorHelper::ProcessMesh(MeshData, AISceneRef, AIMesh);

				if (MeshData.MeshAttrib == EMeshAttribute::None)
				{
					F_GFXLogError("Mesh entry initialization failed.");
					MeshEntries.pop_back();
				}
			}
		}
	}

	FModelProcessorHelper::OnMeshEntriesLoaded(MeshEntries, LoadParams);
}

void FModelProcessor::LoadPhoenix(const FLoadParams& LoadParams)
{
	FInputFileStream InStream{ LoadParams.File , FFile::EFileOpenMode::Binary };
	if (!InStream.is_open())
	{
		F_GFXLogError("Failed to load pmesh model { " << LoadParams.File << " }");
		return;
	}

	FBinaryDeserializer Deserializer{ InStream };
	Deserializer.Serialize(MeshEntries);

	FModelProcessorHelper::OnMeshEntriesLoaded(MeshEntries, LoadParams);
}

const FChar* const FModelProcessorHelper::GetAttributeTypeName(const FbxNodeAttribute::EType Type)
{
	switch (Type)
	{
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		case FbxNodeAttribute::eCachedEffect: return "cached effect";
		case FbxNodeAttribute::eLine: return "line";
	}

	return "unknown";
}

bool FModelProcessorHelper::IsFileFBX(const FChar* const File)
{
	F_Assert(NString::GetLength(File), "Invalid file name.");
	const bool Result = NString::HasExtension(File, "fbx");
	return Result;
}

bool FModelProcessorHelper::IsFileMisc(const FChar* const File)
{
	F_Assert(NString::GetLength(File), "Invalid file name.");
	const bool Result = !(IsFileFBX(File) || IsFilePhoenix(File));
	return Result;
}

bool FModelProcessorHelper::IsFilePhoenix(const FChar* const File)
{
	F_Assert(NString::GetLength(File), "Invalid file name.");
	const bool Result = NString::HasExtension(File, "pmesh");
	return Result;
}

void FModelProcessorHelper::ProcessMesh(
	FMeshData& MeshData,
	const aiScene& AIScene,
	const aiMesh& AIMesh)
{
	F_Assert(MeshData.MeshAttrib, "No model data hints were requested.");

	const bool PositionHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::Positions) != 0;
	const bool NormalsHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::Normals) != 0;
	const bool IndicesHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::Indices) != 0;
	const bool UVCoordsHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::UVCoords) != 0;
	const bool DiffuseMapHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::DiffuseMap) != 0;
	const bool NormalMapHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::NormalMap) != 0;

	const bool HasVertexData = AIMesh.mNumVertices > 0;
	const bool HasIndexData = AIMesh.mNumFaces > 0;
	const bool HasUVCoords = AIMesh.mTextureCoords[0] != nullptr;

	if (!HasVertexData || !HasIndexData)
	{
		F_GFXLogError("This mesh does not have any vertex and/or index data.");
		MeshData.MeshAttrib = EMeshAttribute::None;
		return;
	}

	if (IndicesHintIsSet && !HasIndexData)
	{
		F_GFXLogWarning("Indices were specified but this mesh does not have any.");
		MeshData.MeshAttrib &= ~EMeshAttribute::Indices;
	}

	if (UVCoordsHintIsSet && !HasUVCoords)
	{
		F_GFXLogWarning("UVCoords were specified but this mesh does not have any.");
		MeshData.MeshAttrib &= ~EMeshAttribute::UVCoords;
	}

	if (PositionHintIsSet && HasVertexData)
	{
		const SizeT PositionsArraySize = AIMesh.mNumVertices * FModelProcessorHelper::FloatsPerVertex;
		MeshData.Positions.resize(PositionsArraySize);

		for (UInt32 I = 0; I < AIMesh.mNumVertices; ++I)
		{
			const SizeT Stride = I * FModelProcessorHelper::FloatsPerVertex;

			const SizeT X = Stride;
			const SizeT Y = Stride + 1;
			const SizeT Z = Stride + 2;

			MeshData.Positions[X] = AIMesh.mVertices[I].x;
			MeshData.Positions[Y] = AIMesh.mVertices[I].y;
			MeshData.Positions[Z] = AIMesh.mVertices[I].z;
		}

		MeshData.PositionType = EMeshPositionType::XYZ;
		MeshData.VertexCount = AIMesh.mNumVertices;
	}

	if (NormalsHintIsSet && HasVertexData)
	{
		const SizeT NormalsArraySize = AIMesh.mNumVertices * FModelProcessorHelper::FloatsPerVertex;
		MeshData.Normals.resize(NormalsArraySize);

		for (UInt32 I = 0; I < AIMesh.mNumVertices; ++I)
		{
			const SizeT Stride = I * FModelProcessorHelper::FloatsPerVertex;

			const SizeT X = Stride;
			const SizeT Y = Stride + 1;
			const SizeT Z = Stride + 2;

			MeshData.Normals[X] = AIMesh.mNormals[I].x;
			MeshData.Normals[Y] = AIMesh.mNormals[I].y;
			MeshData.Normals[Z] = AIMesh.mNormals[I].z;
		}
	}

	if (UVCoordsHintIsSet && HasUVCoords)
	{
		F_Assert(AIMesh.mNumUVComponents[0] == FModelProcessorHelper::FloatsPerUVCoord,
			FModelProcessorHelper::FloatsPerUVCoord << " UVCoords were expected, but "
			<< AIMesh.mNumUVComponents[0] << " UVCoords exist instead.");

		const SizeT UVCoordsArraySize = AIMesh.mNumVertices * FModelProcessorHelper::FloatsPerUVCoord;
		MeshData.UVCoords.resize(UVCoordsArraySize);

		for (UInt32 I = 0; I < AIMesh.mNumVertices; ++I)
		{
			const SizeT Stride = I * FModelProcessorHelper::FloatsPerUVCoord;

			const SizeT X = Stride;
			const SizeT Y = Stride + 1;

			MeshData.UVCoords[X] = AIMesh.mTextureCoords[0][I].x;
			MeshData.UVCoords[Y] = AIMesh.mTextureCoords[0][I].y;
		}
	}

	if (IndicesHintIsSet && HasIndexData)
	{
		if (AIMesh.mNumVertices <= TNumericLimits<UInt8>::max())
		{
			ProcessMeshFaces<UInt8>(MeshData, AIScene, AIMesh);
		}
		else if (AIMesh.mNumVertices <= TNumericLimits<UInt16>::max())
		{
			ProcessMeshFaces<UInt16>(MeshData, AIScene, AIMesh);
		}
		else
		{
			ProcessMeshFaces<UInt32>(MeshData, AIScene, AIMesh);
		}
	}

	if (DiffuseMapHintIsSet)
	{
		FModelProcessorHelper::ProcessMaterial(
			MeshData,
			MeshData.TexNameIndices[EMeshDataIndex::Diffuse],
			AIScene,
			AIMesh,
			aiTextureType_DIFFUSE);

		if (MeshData.TexNameIndices[EMeshDataIndex::Diffuse] == 0)
		{
			MeshData.MeshAttrib &= ~EMeshAttribute::DiffuseMap;
		}
	}

	if (NormalMapHintIsSet)
	{
		FModelProcessorHelper::ProcessMaterial(
			MeshData,
			MeshData.TexNameIndices[EMeshDataIndex::Normal],
			AIScene,
			AIMesh,
			aiTextureType_NORMALS);

		if (MeshData.TexNameIndices[EMeshDataIndex::Normal] == 0)
		{
			MeshData.MeshAttrib &= ~EMeshAttribute::NormalMap;
		}
	}
}

template <class IndexT>
void FModelProcessorHelper::ProcessMeshFaces(
	FMeshData& MeshData,
	const aiScene& AIScene,
	const aiMesh& AIMesh)
{
	static_assert(!TNumericLimits<IndexT>::is_signed, "IndexT must be unsigned.");
	static_assert(sizeof(IndexT) >= 1, "sizeof(IndexT) must be >= 1 byte.");
	static_assert(sizeof(IndexT) <= 4, "sizeof(IndexT) must be <= 4 bytes.");

	const SizeT IndicesSize = AIMesh.mNumFaces * FModelProcessorHelper::IndicesPerFace * sizeof(IndexT);

	MeshData.Indices.resize(IndicesSize);
	MeshData.IndexTSize = sizeof(IndexT);

	for (UInt32 I = 0; I < AIMesh.mNumFaces; I++)
	{
		const aiFace& AIFace = AIMesh.mFaces[I];

		F_Assert(AIFace.mNumIndices == IndicesPerFace, "Face data was not triangulated.");
		for (UInt8 J = 0; J < FModelProcessorHelper::IndicesPerFace; ++J)
		{
			F_Assert(AIFace.mIndices[J] <= TNumericLimits<IndexT>::max(), "Index is out of bounds.");
			const SizeT IndicesI = I * FModelProcessorHelper::IndicesPerFace * sizeof(IndexT) + J * sizeof(IndexT);

			UInt8* const UInt8Ptr = &(MeshData.Indices[IndicesI]);
			IndexT* const IndexTPtr = reinterpret_cast<IndexT*>(UInt8Ptr);

			*IndexTPtr = static_cast<IndexT>(AIFace.mIndices[J]);
		}
	}
}

void FModelProcessorHelper::ProcessMaterial(
	FMeshData& MeshData,
	FMeshData::TexNameIndexT& TextureNameIndex,
	const aiScene& AIScene,
	const aiMesh& AIMesh,
	const aiTextureType AITextureType)
{
	F_Assert(AIMesh.mMaterialIndex < AIScene.mNumMaterials,
		"Material index of " << AIMesh.mMaterialIndex << " is over the limit of " << AIScene.mNumMaterials);

	const aiMaterial* const AIMaterial = AIScene.mMaterials[AIMesh.mMaterialIndex];
	F_Assert(AIMaterial, "AIMaterial is null.");

	const UInt32 TextureCount = AIMaterial->GetTextureCount(AITextureType);
	if (TextureCount == 0)
	{
		F_GFXLogWarning("Mesh's material contains 0 diffuse textures.");
		return;
	}

	aiString AIString;
	const aiReturn AIReturn = AIMaterial->GetTexture(AITextureType, 0, &AIString);
	F_Assert(AIReturn == aiReturn_SUCCESS, "Failed to retrieve texture name.");
	F_Assert(AIString.length > 0, "AIString has a non-positive length.");

	AppendTextureName(MeshData, TextureNameIndex, AIString.C_Str(), AIString.length);
}

void FModelProcessorHelper::ProcessMesh(
	FMeshData& MeshData,
	const FbxNode& FBXNode,
	FbxMesh& FBXMesh)
{
	F_Assert(MeshData.MeshAttrib, "No model data hints were requested.");

	const bool PositionHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::Positions) != 0;
	const bool NormalsHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::Normals) != 0;
	const bool IndicesHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::Indices) != 0;
	const bool UVCoordsHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::UVCoords) != 0;
	const bool DiffuseMapHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::DiffuseMap) != 0;
	const bool NormalMapHintIsSet = (MeshData.MeshAttrib & EMeshAttribute::NormalMap) != 0;
	const bool ShouldReadTextureData = DiffuseMapHintIsSet || NormalMapHintIsSet;

	const Int32 ControlPointsCount = FBXMesh.GetControlPointsCount();
	const Int32 TriangleCount = FBXMesh.GetPolygonCount();
	const Int32 VertexCount = TriangleCount * FModelProcessorHelper::VerticesPerTriangle;

	const SizeT PositionsArraySize = VertexCount * FModelProcessorHelper::FloatsPerVertex;
	const SizeT UVCoordsArraySize = VertexCount * FModelProcessorHelper::FloatsPerUVCoord;

	const bool HasVertexData = ControlPointsCount > 0 && TriangleCount > 0;

	if (!HasVertexData)
	{
		F_GFXLogError("The provided FBXMesh does not have any vertex data.");
		MeshData.MeshAttrib = EMeshAttribute::None;
		return;
	}

	if (PositionHintIsSet)
	{
		MeshData.Positions.resize(PositionsArraySize);

		for (Int32 I = 0; I < TriangleCount; ++I)
		{
			const Int32 PolygonSize = FBXMesh.GetPolygonSize(I);
			F_Assert(PolygonSize == FModelProcessorHelper::FloatsPerVertex,
				"Three vertices are required for a triangle, but " << PolygonSize << " were provided.");

			const SizeT Stride =
				I * FModelProcessorHelper::VerticesPerTriangle * FModelProcessorHelper::FloatsPerVertex;

			for (Int32 J = 0; J < FModelProcessorHelper::FloatsPerVertex; ++J)
			{
				const SizeT Offset = J * FModelProcessorHelper::FloatsPerVertex;
				const SizeT StrideOffsetSum = Stride + Offset;

				const SizeT X = StrideOffsetSum;
				const SizeT Y = StrideOffsetSum + 1;
				const SizeT Z = StrideOffsetSum + 2;

				const Int32 Index = FBXMesh.GetPolygonVertex(I, J);
				F_Assert(Index >= 0 && Index < ControlPointsCount, "Index is out of bounds.");

				MeshData.Positions[X] = static_cast<FMeshData::PositionT>(FBXMesh.mControlPoints[Index][0]);
				MeshData.Positions[Y] = static_cast<FMeshData::PositionT>(FBXMesh.mControlPoints[Index][1]);
				MeshData.Positions[Z] = static_cast<FMeshData::PositionT>(FBXMesh.mControlPoints[Index][2]);
			}
		}

		MeshData.PositionType = EMeshPositionType::XYZ;
		MeshData.VertexCount = VertexCount;
	}

	if (NormalsHintIsSet)
	{
		MeshData.Normals.resize(PositionsArraySize);

		for (Int32 I = 0; I < TriangleCount; ++I)
		{
			F_Assert(FBXMesh.GetPolygonSize(I) == FModelProcessorHelper::FloatsPerVertex,
				"Three vertices are required for a triangle.");

			const SizeT Stride = I * FModelProcessorHelper::VerticesPerTriangle * 
				FModelProcessorHelper::FloatsPerVertex;

			for (Int32 J = 0; J < FModelProcessorHelper::FloatsPerVertex; ++J)
			{
				const SizeT Offset = J * FModelProcessorHelper::FloatsPerVertex;
				const SizeT StrideOffsetSum = Stride + Offset;

				const SizeT X = StrideOffsetSum;
				const SizeT Y = StrideOffsetSum + 1;
				const SizeT Z = StrideOffsetSum + 2;

				FbxVector4 Normal;
				const bool bIsNormalRetrieved = FBXMesh.GetPolygonVertexNormal(I, J, Normal);
				F_Assert(bIsNormalRetrieved, "Failed to retrieve normal.");

				MeshData.Normals[X] = static_cast<FMeshData::NormalT>(Normal[0]);
				MeshData.Normals[Y] = static_cast<FMeshData::NormalT>(Normal[1]);
				MeshData.Normals[Z] = static_cast<FMeshData::NormalT>(Normal[2]);
			}
		}
	}

	if (UVCoordsHintIsSet)
	{
		TRawPtr<FbxLayerElementArrayTemplate<FbxVector2>> UVCoordsArray = nullptr;
		FBXMesh.GetTextureUV(&UVCoordsArray.Get(), FbxLayerElement::eTextureDiffuse);
		MeshData.UVCoords.resize(UVCoordsArraySize);

		if (!UVCoordsArray.IsValid())
		{
			F_GFXLogWarning("Failed to get UV coordinates for a mesh.");
			std::fill(MeshData.UVCoords.begin(), MeshData.UVCoords.end(), 0.f);
		}
		else
		{
			for (Int32 I = 0; I < TriangleCount; ++I)
			{
				F_Assert(FBXMesh.GetPolygonSize(I) == FModelProcessorHelper::FloatsPerVertex,
					"Three vertices are required for a triangle.");

				const SizeT Stride = I * FModelProcessorHelper::VerticesPerTriangle * 
					FModelProcessorHelper::FloatsPerUVCoord;

				for (Int32 J = 0; J < FModelProcessorHelper::FloatsPerVertex; ++J)
				{
					const SizeT Offset = J * FModelProcessorHelper::FloatsPerUVCoord;
					const SizeT StrideOffsetSum = Stride + Offset;

					const SizeT X = StrideOffsetSum;
					const SizeT Y = StrideOffsetSum + 1;

					const Int32 Index = FBXMesh.GetTextureUVIndex(I, J);
					F_Assert(Index >= 0 && Index < UVCoordsArray->GetCount(), "Index is out of bounds.");

					FbxVector2 UVCoord = UVCoordsArray->GetAt(Index);

					FMeshData::UVCoordT XValue = static_cast<FMeshData::UVCoordT>(UVCoord[0]);
					FMeshData::UVCoordT YValue = static_cast<FMeshData::UVCoordT>(UVCoord[1]);

					const Float32 MaxUVCoordValue = 1.f;
					if (FMathf::Abs(XValue) > MaxUVCoordValue)
					{
						XValue = FMathf::Modulo(XValue, MaxUVCoordValue);
					}

					if (FMathf::Abs(YValue) > MaxUVCoordValue)
					{
						YValue = FMathf::Modulo(YValue, MaxUVCoordValue);
					}

					YValue = MaxUVCoordValue - YValue;

					MeshData.UVCoords[X] = XValue;
					MeshData.UVCoords[Y] = YValue;
				}
			}
		}
	}

	if (IndicesHintIsSet)
	{
		// #FIXME: The FBX SDK doesn't seem to expose
		// index data for all of the types, so eventually,
		// duplicate data will have to be removed and indices
		// will have to be generated accordingly, and then
		// the indices will have to be sorted appropriately
		// for later.
		typedef UInt32 IndexDataTypeT;

		const UInt32 VertexCountUInt32 = static_cast<UInt32>(VertexCount);
		const SizeT IndexArraySize = VertexCountUInt32 * sizeof(IndexDataTypeT);

		MeshData.Indices.resize(IndexArraySize);
		MeshData.IndexTSize = sizeof(IndexDataTypeT);

		for (SizeT I = 0; I < VertexCountUInt32; ++I)
		{
			const SizeT IndicesI = I * sizeof(IndexDataTypeT);

			FMeshData::IndexT* const IndexTPtr = &(MeshData.Indices[IndicesI]);
			IndexDataTypeT* const IndexDataTypeTPtr = reinterpret_cast<IndexDataTypeT*>(IndexTPtr);

			*IndexDataTypeTPtr = static_cast<IndexDataTypeT>(I);
		}
	}

	if (ShouldReadTextureData)
	{
		const Int32 MaterialObjectCount = FBXNode.GetSrcObjectCount<FbxSurfaceMaterial>();
		F_GFXLogWarningIf(MaterialObjectCount > 1, "1 material was expected, but " << MaterialObjectCount << " exist.");

		if (MaterialObjectCount > 0)
		{
			const FbxSurfaceMaterial* const FBXSurfaceMaterial = FBXNode.GetSrcObject<FbxSurfaceMaterial>(0);
			if (FBXSurfaceMaterial)
			{
				static_assert(EMeshDataIndex::Count == 2, "This section requires updating.");
				if (DiffuseMapHintIsSet)
				{
					ProcessMaterial(
						MeshData,
						MeshData.TexNameIndices[EMeshDataIndex::Diffuse],
						*FBXSurfaceMaterial,
						FbxSurfaceMaterial::sDiffuse);
				}

				if (NormalsHintIsSet)
				{
					ProcessMaterial(
						MeshData,
						MeshData.TexNameIndices[EMeshDataIndex::Normal],
						*FBXSurfaceMaterial,
						FbxSurfaceMaterial::sNormalMap);
				}
			}
		}

		static_assert(EMeshDataIndex::Count == 2, "This section requires updating.");
		if (MeshData.TexNameIndices[EMeshDataIndex::Diffuse] == 0)
		{
			MeshData.MeshAttrib &= ~EMeshAttribute::DiffuseMap;
		}

		if (MeshData.TexNameIndices[EMeshDataIndex::Normal] == 0)
		{
			MeshData.MeshAttrib &= ~EMeshAttribute::NormalMap;
		}
	}
}

void FModelProcessorHelper::ProcessMaterial(
	FMeshData& MeshData,
	FMeshData::TexNameIndexT& TextureNameIndex,
	const FbxSurfaceMaterial& FBXSurfaceMaterial,
	const FChar* const PropertyName)
{
	const FbxProperty FBXProperty = FBXSurfaceMaterial.FindProperty(PropertyName);

	const Int32 LayeredTextureCount = FBXProperty.GetSrcObjectCount<FbxLayeredTexture>();
	F_GFXLogWarningIf(LayeredTextureCount > 1, "1 layered texture was expected, but " << LayeredTextureCount << " exist.");
	F_DebugBreakIf(LayeredTextureCount > 1);

	if (LayeredTextureCount > 0)
	{
		const FbxObject* const FBXPropertyObject = FBXProperty.GetSrcObject(0);
		const FbxLayeredTexture* const FBXLayeredTexture = FbxCast<FbxLayeredTexture>(FBXPropertyObject);
		F_Assert(FBXLayeredTexture, "FBXLayeredTexture is null.");

		const Int32 TextureCount = FBXLayeredTexture->GetSrcObjectCount<FbxTexture>();
		F_GFXLogWarningIf(TextureCount > 1, "1 texture was expected, but " << TextureCount << " exist.");
		F_DebugBreakIf(TextureCount > 1);

		if (TextureCount > 0)
		{
			const FbxObject* const FBXLayeredTextureObj = FBXLayeredTexture->GetSrcObject(0);
			const FbxFileTexture* const FBXFileTexture = FbxCast<FbxFileTexture>(FBXLayeredTextureObj);
			F_Assert(FBXFileTexture, "FBXFileTexture is null.");

			const FChar* const TextureName = FBXFileTexture->GetFileName();
			const SizeT TextureNameLength = NString::GetLength(TextureName);
			F_Assert(TextureName, "TextureName is null.");

			AppendTextureName(MeshData, TextureNameIndex, TextureName, TextureNameLength);
		}
	}
	else if (LayeredTextureCount == 0)
	{
		const Int32 TextureCount = FBXProperty.GetSrcObjectCount<FbxTexture>();
		F_GFXLogWarningIf(TextureCount > 1, "1 texture was expected, but " << TextureCount << " exist.");
		F_DebugBreakIf(TextureCount > 1);

		if (TextureCount > 0)
		{
			const FbxObject* const FBXPropertyObj = FBXProperty.GetSrcObject(0);
			const FbxFileTexture* const FBXFileTexture = FbxCast<FbxFileTexture>(FBXPropertyObj);
			F_Assert(FBXFileTexture, "FBXFileTexture is null.");

			const FChar* const TextureName = FBXFileTexture->GetFileName();
			const SizeT TextureNameLength = NString::GetLength(TextureName);
			F_Assert(TextureName, "TextureName is null.");

			AppendTextureName(MeshData, TextureNameIndex, TextureName, TextureNameLength);
		}
	}
}

void FModelProcessorHelper::AppendTextureName(
	FMeshData& MeshData,
	FMeshData::TexNameIndexT& TextureNameIndex,
	const FChar* const Str,
	const SizeT StrLength)
{
	TVector<FChar>& TextureNames = MeshData.TextureNames;

	if (MeshData.TextureNames.size() == 0)
	{
		MeshData.TextureNames.push_back('\0');
	}

	const SizeT InitialTexNamesSize = TextureNames.size();
	F_Assert(InitialTexNamesSize < TNumericLimits<FMeshData::TexNameIndexT>::max(),
		"Initial index is too large to store in a FMeshData::TexNameIndexT");

	SizeT CharPos = NString::FindLastOf(Str, StrLength, "/\\");
	CharPos = CharPos != FString::npos ? CharPos + 1 : 0;
	F_Assert(CharPos < StrLength,
		"Character position of " << CharPos << " should be less than " << StrLength);

	const SizeT UsedLength = StrLength - CharPos;

	TextureNameIndex = static_cast<FMeshData::TexNameIndexT>(InitialTexNamesSize);
	TextureNames.resize(InitialTexNamesSize + UsedLength + 1);
	std::memcpy(&TextureNames[TextureNameIndex], &Str[CharPos], UsedLength);
	TextureNames[TextureNames.size() - 1] = '\0';
}

void FModelProcessorHelper::OnMeshEntriesLoaded(
	FMeshData::FEntries& MeshEntries,
	const FModelProcessor::FLoadParams& LoadParams)
{
	FModelProcessorHelper::ProcessOriginOffset(MeshEntries, LoadParams.OriginOffset);
	FModelProcessorHelper::ProcessSize(MeshEntries, LoadParams.NormalizedSize);
	F_GFXLog("Model created: " << NString::ExtractFileName(LoadParams.File));
}

void FModelProcessorHelper::ProcessOriginOffset(FMeshData::FEntries& MeshEntries, const FVector3D& OriginOffset)
{
	if (OriginOffset.x == 0.f && OriginOffset.y == 0.f && OriginOffset.z == 0.f)
	{
		return;
	}

	const auto Stride = FModelProcessorHelper::FloatsPerVertex;
	for (auto& MeshData : MeshEntries)
	{
		for (SizeT I = 0, Max = MeshData.Positions.size(); I < Max; I += Stride)
		{
			MeshData.Positions[I] += OriginOffset.x;
			MeshData.Positions[I + 1] += OriginOffset.y;
			MeshData.Positions[I + 2] += OriginOffset.z;
		}
	}
}

void FModelProcessorHelper::ProcessSize(FMeshData::FEntries& MeshEntries, const Float32 NormalizedSize)
{
	if (NormalizedSize == 0.f)
	{
		return;
	}

	F_Assert(NormalizedSize > 0.f, "Normalized size can not be negative.");
	Float32 HighestLengthSq = 0.f;

	const auto Stride = FModelProcessorHelper::FloatsPerVertex;
	for (const auto& MeshEntry : MeshEntries)
	{
		const auto& Positions = MeshEntry.Positions;
		for (SizeT I = 0, Max = Positions.size(); I < Max; I += Stride)
		{
			const FVector3D Vertex(Positions[I], Positions[I + 1], Positions[I + 2]);
			const Float32 LengthSq = NVector3D::GetLengthSq(Vertex);
			HighestLengthSq = FMathf::Max(HighestLengthSq, LengthSq);
		}
	}

	F_Assert(HighestLengthSq > 0.f, "Length squared must be a positive non-zero number.");
	const Float32 Length = std::sqrtf(HighestLengthSq);
	const Float32 Multiplicand = NormalizedSize / Length;

	for (auto& MeshEntry : MeshEntries)
	{
		for (SizeT I = 0, Max = MeshEntry.Positions.size(); I < Max; ++I)
		{
			MeshEntry.Positions[I] *= Multiplicand;
		}
	}
}
