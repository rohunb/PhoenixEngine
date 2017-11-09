#include "Stdafx.h"
#include "Rendering/GFXScene.h"

#include "Rendering/GFXEngine.h"
#include "Rendering/Caches/GFXCaches.h"
#include "Rendering/Debug/GFXDebug.h"
#include "Rendering/Handles/GFXHandles.h"

using namespace Phoenix;

FGFXScene::~FGFXScene()
{
	DeInit();
}

void FGFXScene::Init()
{
	DeInit();

	PersCam = CreateCamera();
	OrthoCam = CreateCamera();
}

bool FGFXScene::IsValid() const
{
	const bool Result = PersCam.IsValid() && OrthoCam.IsValid();
	return Result;
}

void FGFXScene::DeInit()
{
	OrthoCam.DeInit();
	PersCam.DeInit();

#define F_GFXSceneDebugInstances(Data) F_GFXLogErrorIf(Data.GetDataSize(), "DeInit was called but " << Data.GetDataSize() << " " << #Data << " refs remain.");

	F_GFXSceneDebugInstances(ImageInstances);
	F_GFXSceneDebugInstances(ModelInstances);
	F_GFXSceneDebugInstances(TextInstances);
	F_GFXSceneDebugInstances(CameraHandles);

#undef F_GFXSceneDebugInstances
}

void FGFXScene::SetOrthographicCamera(const THandle<FCamera>& Camera)
{
	OrthoCam = Camera;
}

void FGFXScene::SetPerspectiveCamera(const THandle<FCamera>& Camera)
{
	PersCam = Camera;
}

THandle<FCamera>& FGFXScene::GetOrthographicCamera()
{
	return OrthoCam;
}

const THandle<FCamera>& FGFXScene::GetOrthographicCamera() const
{
	return OrthoCam;
}

THandle<FCamera>& FGFXScene::GetPerspectiveCamera()
{
	return PersCam;
}

const THandle<FCamera>& FGFXScene::GetPerspectiveCamera() const
{
	return PersCam;
}

THandle<FCamera> FGFXScene::CreateCamera()
{
	THandle<FCamera> CameraHandle = CameraHandles.CreateHandle();
	return CameraHandle;
}

THandle<FImageInstance> FGFXScene::CreateImage(
	const FString& ImageFileName,
	const EImageInfo::Type ImageInfo)
{
	THandle<FImageInstance> IIHandle = ImageInstances.CreateHandle();
	IIHandle->SetImage(ImageFileName);
	return IIHandle;
}

THandle<FModelInstance> FGFXScene::CreateModel(
	const FString& ModelFileName,
	const FMaterial& Material,
	const EModelInfo::Type ModelInfo)
{
	THandle<FModelInstance> MIHandle = ModelInstances.CreateHandle();
	MIHandle->SetModel(ModelFileName);
	MIHandle->SetMaterial(Material);
	return MIHandle;
}

THandle<FTextInstance> FGFXScene::CreateText(const FString& FontFileName)
{
	THandle<FTextInstance> T2DIHandle = TextInstances.CreateHandle();
	T2DIHandle->SetFont(FontFileName);
	return T2DIHandle;
}

const FImageInstances& FGFXScene::GetImageInstances() const
{
	return ImageInstances;
}

const FModelInstances& FGFXScene::GetModelInstances() const
{
	return ModelInstances;
}

const FTextInstances& FGFXScene::GetTextInstances() const
{
	return TextInstances;
}
