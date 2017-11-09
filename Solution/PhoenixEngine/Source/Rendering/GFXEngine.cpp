#include "Stdafx.h"
#include "Rendering/GFXEngine.h"

#include "Config/GFXCompileConfig.h"
#include "ExternalLib/FreeTypeIncludes.h"
#include "ExternalLib/GLEWIncludes.h"
#include "ExternalLib/GLIncludes.h"
#include "Utility/Containers/Array.h"
#include "Utility/Containers/PriorityQueue.h"
#include "Utility/Containers/UnorderedSet.h"
#include "Utility/FileIO/File.h"
#include "Utility/MetaProgramming/AssertOnCopy.h"
#include "Utility/Misc/Memory.h"
#include "Utility/Misc/Timer.h"
#include "Utility/Threading/AsyncTaskHandler.h"
#include "Utility/Threading/Atomic.h"
#include "Utility/Threading/ConditionVariable.h"
#include "Utility/Threading/Thread.h"
#include "Math/Math.h"
#include "Math/MathCommon.h"
#include "Math/Matrix3D.h"
#include "Math/MatrixTransform.h"
#include "Math/Vector4D.h"
#include "Platform/Window/IWindow.h"
#include "Rendering/Camera.h"
#include "Rendering/GBuffer.h"
#include "Rendering/GFXRenderData.h"
#include "Rendering/GFXScene.h"
#include "Rendering/GFXUtils.h"
#include "Rendering/Caches/GFXCaches.h"
#include "Rendering/Debug/GFXDebug.h"
#include "Rendering/Debug/GFXProfiler.h"
#include "Rendering/GL/GLInterface.h"
#include "Rendering/Handles/GFXHandles.h"
#include "Rendering/Image/Image.h"
#include "Rendering/Image/ImageProcessor.h"
#include "Rendering/Model/Model.h"
#include "Rendering/Model/ModelInstance.h"
#include "Rendering/Model/ModelProcessor.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/Shader/ShaderNames.h"
#include "Rendering/Shader/ShaderUniformNames.h"
#include "Rendering/Text/Font.h"
#include "Rendering/Text/FontEngine.h"
#include "Rendering/Threading/GFXTaskReceiver.h"

#ifndef PHOENIX_GFX_COMPILE_CONFIG
#	error("Config/GFXCompileConfig.h should be included in this file.")
#endif

using namespace Phoenix;
using namespace Phoenix::GL;

namespace Phoenix
{
	typedef std::pair<const FModelInstance*, THandle<FModel>> FModelRenderEntry;
	typedef std::pair<const FImageInstance*, THandle<FImage>> FImageRenderEntry;

	typedef TVector<FModelRenderEntry> FModelRenderList;
	typedef TVector<FImageRenderEntry> FImageRenderList;

	struct FGFXEngineInternals
	{
		/* GFXThread: Read/Write, GameThread: N/A */
#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
		/*! \brief The thread that runs the graphics pipeline when multithreaded rendering is enabled. */
		FSafeThread Thread;
#endif
		/*! \brief The parameters used to initialize the graphics engine. */
		FGFXEngine::FInitParams InitParams;
		/*! \brief The function that's used to make the necessary draw calls. */
		FGFXEngine::DrawFunction DrawFunc{ &FGFXEngine::ThreadEmptyFunction };

		/*! \brief Contains everything that'll be rendered on screen. */
		TUniquePtr<FGFXRenderData> RenderData;
		/*! \brief Contains all of the handles for the engine. */
		TUniquePtr<FGFXHandles> Handles;
		/*! \brief Contains all of the caches for the engine. */
		TUniquePtr<FGFXCaches> Caches;
		/*! \brief Responsible for most font related matters. */
		TUniquePtr<FFontEngine> FontEngine;

		/*! \brief Contains functonality for streaming in data. */
		TUniquePtr<FAsyncTaskHandler> TaskHandler;
		TUniquePtr<FGFXTaskReceiver> MsgReceiver;
		TUnorderedSet<FString> PendingAsyncTasks;
		FGFXTaskReceiver::FTasks Tasks;

		/*! \brief Standard multi-render target buffer for deferred shading. */
		FGBuffer GBuffer;
		/*! \brief A full screen quad. */
		FMesh MeshQuad;
		/*! \brief Contains the ModelDeferred series of shaders. */
		THandle<FShader> MDShader;
		/*! \brief Contains the ModelDeferredFinal series of shaders. */
		THandle<FShader> MDFShader;
		/*! \brief Contains the Image2D series of shaders. */
		THandle<FShader> I2DShader;
		/*! \brief Contains the Text2D series of shaders. */
		THandle<FShader> T2DShader;

		/* GFXThread: Read/Write, GameThread: Read */
		/*! \brief Determines whether or not the engine is running. */
		TAtomic<bool> IsRunning{ false };
		/*! \brief Determines whether or not the engine has been shut down. */
		TAtomic<bool> IsShutDown{ false };

		/* GFXThread: N/A, GameThread: Read/Write */
		/*! \brief Represents the scene that the game thread interacts with. */
		TUniquePtr<FGFXScene> Scene;

		/* GFXThread: Read/Write, GameThread: Read/Write */
#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
		/*! \brief Used to lock access to certain data within the engine. */
		FMutex Mutex;
		/*! \brief Used to wait until a signal is sent that new data has been passed to the engine. */
		FConditionVariable ConditionVariable;
		/*! \brief Represents the grapics state that will be rendered. */
		TUniquePtr<FGFXRenderData> GameRenderData;
		/*! \brief Represents the grapics state that is waiting to be rendered. */
		TUniquePtr<FGFXRenderData> QueuedRenderData;
#endif
	};

	namespace FGFXHelper
	{
		bool InitScene(FGFXScene& Scene, const FGFXEngine::FInitParams& InitParams);

		THandle<FImage> LoadAndCacheImage(
			FGFXCaches& GFXCaches,
			FGFXHandles& GFXHandles,
			FString&& TexFileName,
			const FChar* const RelativeDirPath,
			const EPixelFormat::Value PixelFormat);

		THandle<FImage> LoadAndCacheImageGL(
			FImageCache& ImageCache,
			FImageHandles& ImageHandles,
			FString&& ImageFileName,
			const FImageProcessor& ImageProcessor);

		THandle<FModel> LoadAndCacheModel(
			FGFXCaches& GFXCaches,
			FGFXHandles& GFXHandles,
			FString&& ModelFileName,
			const FChar* const RelativeDirPath,
			const EMeshAttribute::Type MeshAttributes);

		THandle<FModel> LoadAndCacheModelGL(
			FGFXCaches& GFXCaches,
			FGFXHandles& GFXHandles,
			FString&& ModelFileName,
			const FModelProcessor& ModelProcessor);

		THandle<FShader> LoadAndCacheShader(
			FGFXCaches& GFXCaches,
			FGFXHandles& GFXHandles,
			const FShader::FCodePtrArray& ShaderFileNames,
			const FChar* const RelativeDirPath,
			const FChar* const ShaderKey);

		void RenderModels(
			const FGFXEngineInternals& Eng,
			const FModelRenderList& ModelRenderList,
			const FMatrix4D& ViewProjectionMatrix);
	};

	namespace FGFXAsyncTasks
	{
		void ProcessModel(FGFXEngineInternals& Eng, const FString& ModelName);

		void ProcessImage(FGFXEngineInternals& Eng, const FString& ImageName);
	};
}

FGFXEngine::FGFXEngine()
	: PImplData(std::make_unique<FGFXEngineInternals>())
{
}

FGFXEngine::~FGFXEngine()
{
#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
	auto& Eng = Get();
	Eng.Thread.Join();
#endif
}

void FGFXEngine::Init(const FInitParams& InitParams)
{
	F_GFXProfile();
	auto& Eng = Get();

	F_Assert(InitParams.Window.IsValid(), "Initialization parameters must be valid.");
	F_Assert(!Eng.IsRunning.load(), "GFX Thread is already running.");

	Eng.InitParams = InitParams;
	Eng.RenderData = std::make_unique<FGFXRenderData>();

#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
	Eng.GameRenderData = std::make_unique<FGFXRenderData>();
	Eng.QueuedRenderData = std::make_unique<FGFXRenderData>();
#endif

	Eng.Scene = std::make_unique<FGFXScene>();

	const bool bIsSceneInit = FGFXHelper::InitScene(*Eng.Scene, Eng.InitParams);
	if (!bIsSceneInit)
	{
		F_GFXLogError("Failed to initialize scene.");
		return;
	}

	Eng.IsRunning = true;

#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
	Eng.Thread = FThread(&FGFXEngine::ThreadRun, this);
#else
	ThreadInit();
#endif
}

bool FGFXEngine::IsValid() const
{
	auto& Eng = Get();
	const bool LocalIsRunning = Eng.IsRunning.load();
	return LocalIsRunning;
}

void FGFXEngine::ForceShutDown()
{
	auto& Eng = Get();
	Eng.IsRunning = false;

#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
	TUniqueLock<FMutex> Lock(Eng.Mutex);
	Eng.ConditionVariable.notify_one();
#else
	ThreadDeInit();
#endif
}

bool FGFXEngine::IsShutDown() const
{
	auto& Eng = Get();
	const bool LocalIsShutDown = Eng.IsShutDown.load();
	return LocalIsShutDown;
}

void FGFXEngine::SetUpNextRender()
{
	auto& Eng = Get();

	{
		F_Profile();
		auto& Scene = GetScene();

#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
		auto& RenderData = *Eng.GameRenderData;
#else
		auto& RenderData = *Eng.RenderData;
#endif

		const auto& MIs = Scene.GetModelInstances();
		const auto& IIs = Scene.GetImageInstances();

		RenderData.PersCam = *Scene.GetPerspectiveCamera();
		RenderData.OrthoCam = *Scene.GetOrthographicCamera();

		RenderData.Images = IIs.GetData();
		RenderData.Models = MIs.GetData();

		RenderData.IsReadyForRender = true;

#if !PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
		ThreadDraw();
#endif
	}

	{
#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
		TUniqueLock<FMutex> Lock(Eng.Mutex);
		std::swap(Eng.QueuedRenderData, Eng.GameRenderData);
		Eng.ConditionVariable.notify_one();
#endif
	}
}

FGFXScene& FGFXEngine::GetScene()
{
	auto& Eng = Get();
	return *Eng.Scene;
}

const FGFXScene& FGFXEngine::GetScene() const
{
	const auto& Eng = Get();
	return *Eng.Scene;
}

void FGFXEngine::ThreadRun()
{
#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
	ThreadInit();

	auto& Eng = Get();

	while (Eng.IsRunning)
	{
		ThreadDraw();

		{
			TUniqueLock<FMutex> Lock(Eng.Mutex);
			Eng.ConditionVariable.wait(Lock);
			std::swap(Eng.RenderData, Eng.QueuedRenderData);
		}
	}

	ThreadDeInit();
#else
	F_Assert(false, "This function should not be called in this render mode.");
#endif
}

void FGFXEngine::ThreadInit()
{
	F_GFXProfile();
	F_GFXLog("GFX Thread ID: " << NThread::GetCallingThreadID());
	auto& Eng = Get();

	// Note: An #undef exists for this define.
#define F_GFXEngineOnInitError(Condition, Error)	\
	if (Condition)									\
	{												\
		F_GFXLogError(Error);						\
		ThreadDeInit();								\
		return;										\
	}

	if (!Eng.Handles)
	{
		Eng.Handles = std::make_unique<FGFXHandles>();
	}

	Eng.Caches = std::make_unique<FGFXCaches>();

	if (!Eng.FontEngine)
	{
		Eng.FontEngine = std::make_unique<FFontEngine>();
	}

	if (!Eng.MsgReceiver)
	{
		Eng.MsgReceiver = std::make_unique<FGFXTaskReceiver>();
		Eng.MsgReceiver->Init();
	}

	if (!Eng.TaskHandler)
	{
		FAsyncTaskHandler::FInitParams InitParams;

#if PHOENIX_GFX_USE_MAX_THREADS_FOR_LOADING
		InitParams.MaxThreadCountHint = NThread::GetHardwareThreadCount();
#else
		// Assuming 1 game thread, 1 gfx thread (potentially), and at least a thread for audio.
#	if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
		const UInt32 ThreadsBusyCount = 3;
#	else
		const UInt32 ThreadsBusyCount = 2;
#	endif

		// Use the maximum number of threads for now.
		const UInt32 HardwareThreadCount = NThread::GetHardwareThreadCount();
		const bool AreExtraThreadsAvail = HardwareThreadCount > ThreadsBusyCount;

		InitParams.MaxThreadCountHint = AreExtraThreadsAvail ? HardwareThreadCount - ThreadsBusyCount : 1;
#endif

		Eng.TaskHandler = std::make_unique<FAsyncTaskHandler>();
		Eng.TaskHandler->Init(InitParams);
		Eng.TaskHandler->Run();

		F_Assert(Eng.TaskHandler->IsValid(), "Failed to initialize task handler.");
	}

#if PHOENIX_FREE_TYPE_AVAILABLE
	Eng.FontEngine->Init(this);
	F_GFXEngineOnInitError(!Eng.FontEngine->IsValid(), "Failed to initialize font engine.");
#endif

	Eng.InitParams.Window->SetGraphicsContextCurrent();
	Eng.InitParams.Window->SetSwapInterval(1);

	glewExperimental = true;

	const bool bGlewFailedToInit = glewInit() != GLEW_OK;
	F_GFXEngineOnInitError(bGlewFailedToInit, "Failed to initialize GLEW.");

	F_GLIgnoreErrors();
	const bool bAreShadersInit = ThreadInitShaders();
	F_GFXEngineOnInitError(!bAreShadersInit, "Failed to initialize shaders.");

	const GLsizei Width = static_cast<GLsizei>(Eng.InitParams.Window->GetDimensions().x);
	const GLsizei Height = static_cast<GLsizei>(Eng.InitParams.Window->GetDimensions().y);

	F_GLDisplayErrors();
	Eng.GBuffer.Init(Width, Height);
	F_GFXEngineOnInitError(!Eng.GBuffer.IsValid(), "Failed to initialize framebuffer.");

	F_GLDisplayErrors();
	FMeshData MeshQuadData = FMeshData::CreateQuadMesh();
	Eng.MeshQuad.Init(MeshQuadData, Eng.Caches->GetImageCache());
	F_GFXEngineOnInitError(!Eng.MeshQuad.IsValid(), "Failed to initialize mesh quad.");

	Eng.InitParams.Window->Show();

	F_GLDisplayErrors();
	F_GL(GL::Viewport(0, 0, Width, Height));
	F_GL(GL::ClearColor(0.f, 0.f, 0.f, 1.f));
	F_GL(GL::Clear(EClearBit::Color));
	F_GL(GL::Enable(ECapability::DepthTest));

	Eng.InitParams.Window->BufferSwap();

	Eng.DrawFunc = &FGFXEngine::ThreadDrawInternal;

	GFXUtils::LogOpenGLInfo();
	Eng.IsShutDown = false;
#undef F_GFXEngineOnInitError
}

void FGFXEngine::ThreadDeInit()
{
	auto& Eng = Get();

	Eng.T2DShader.DeInit();
	Eng.I2DShader.DeInit();
	Eng.MDFShader.DeInit();
	Eng.MDShader.DeInit();

	Eng.MeshQuad.DeInit();
	Eng.GBuffer.DeInit();

	Eng.Caches.reset();

	if (Eng.Handles)
	{
		Eng.Handles->LogErrorsIfReferencesRemain();
		Eng.Handles->ForceClearResources();
	}

	if (Eng.TaskHandler)
	{
		Eng.TaskHandler->ForceShutDown();

		// Wait for other threads to shut down.
		while (!Eng.TaskHandler->IsShutDown())
		{
			NThread::SleepThread(1);
		}
	}

	Eng.PendingAsyncTasks.clear();
	Eng.Tasks.clear();

	if (Eng.MsgReceiver)
	{
		Eng.MsgReceiver->DeInit();
	}

	if (Eng.FontEngine)
	{
		Eng.FontEngine->DeInit();
	}

	Eng.DrawFunc = &FGFXEngine::ThreadEmptyFunction;
	Eng.InitParams.Window = nullptr;
	Eng.IsShutDown = true;
}

void FGFXEngine::ThreadDraw()
{
	auto& Eng = Get();

	F_Assert(Eng.DrawFunc, "DrawFunc is null.");
	(this->*Eng.DrawFunc)();
}

THandle<FFont> FGFXEngine::ThreadLoadAndCacheFont(const FChar* const FontFileName)
{
	auto& Eng = Get();
	F_Assert(FontFileName, "FontFileName is null.");

	THandle<FFont> Font = Eng.FontEngine->LoadAndCacheFont(FontFileName);
	F_GFXLogErrorIf(!Font.IsValid(), "Failed to load and cache font { " << FontFileName << " }");

	return Font;
}

THandle<FImage> FGFXEngine::ThreadLoadAndCacheImage(const FChar* const ImageFileName)
{
	auto& Eng = Get();
	F_Assert(ImageFileName, "ImageFileName is null.");

	FString ImageFileNameCopy = ImageFileName;
	F_Assert(!ImageFileNameCopy.empty(), "The string pointed to by ImageFileName is empty.");

	THandle<FImage> Image = FGFXHelper::LoadAndCacheImage(
		*Eng.Caches,
		*Eng.Handles,
		std::move(ImageFileNameCopy),
		EAssetPath::Get(EAssetPath::Textures),
		EPixelFormat::RGBA);

	F_GFXLogErrorIf(!Image.IsValid(), "Failed to load and cache image { " << ImageFileName << " }");
	return Image;
}

THandle<FModel> FGFXEngine::ThreadLoadAndCacheModel(const FChar* const ModelFileName)
{
	auto& Eng = Get();
	F_Assert(ModelFileName, "ModelFileName is null.");

	FString ModelFileNameCopy = ModelFileName;
	F_Assert(!ModelFileNameCopy.empty(), "The string pointed to by ModelFileName is empty.");

	THandle<FModel> Model = FGFXHelper::LoadAndCacheModel(
		*Eng.Caches,
		*Eng.Handles,
		std::move(ModelFileNameCopy),
		EAssetPath::Get(EAssetPath::Models),
		EMeshAttribute::All);

	F_GFXLogErrorIf(!Model.IsValid(), "Failed to load and cache model { " << ModelFileName << " }");
	return Model;
}

FGFXHandles& FGFXEngine::GetHandles()
{
	auto& Eng = Get();
	return *Eng.Handles;
}

const FGFXHandles& FGFXEngine::GetHandles() const
{
	const auto& Eng = Get();
	return *Eng.Handles;
}

FGFXCaches& FGFXEngine::GetCaches()
{
	auto& Eng = Get();
	return *Eng.Caches;
}

const FGFXCaches& FGFXEngine::GetCaches() const
{
	const auto& Eng = Get();
	return *Eng.Caches;
}

FGFXEngineInternals& FGFXEngine::Get()
{
	return *PImplData;
}

const FGFXEngineInternals& FGFXEngine::Get() const
{
	return *PImplData;
}

void FGFXEngine::ThreadDrawInternal()
{
	F_Assert(IsValid(), "This class is invalid.");
	ThreadDrawScene();
}

void FGFXEngine::ThreadEmptyFunction()
{
}

bool FGFXEngine::ThreadInitShaders()
{
	auto& Eng = Get();

	GL::VertexArrayT VertexArray = 0;
	F_GL(GL::GenVertexArrays(1, &VertexArray));
	F_GL(GL::BindVertexArray(VertexArray));

	const FChar* const RelativeDirPath = EAssetPath::Get(EAssetPath::Shaders);

	static_assert(EShaderIndex::Vertex == 1, "This function requires updating.");
	static_assert(EShaderIndex::Fragment == 5, "This function requires updating.");

	const SizeT ArraySize = 4;
	const TArray<FShader::FCodePtrArray, ArraySize> CodeArray =
	{
		FShader::FCodePtrArray { nullptr, "ModelDeferred_VS.txt", nullptr, nullptr, nullptr, "ModelDeferred_FS.txt" },
		FShader::FCodePtrArray { nullptr, "ModelDeferredFinal_VS.txt", nullptr, nullptr, nullptr, "ModelDeferredFinal_FS.txt" },
		FShader::FCodePtrArray{ nullptr, "Image2D_VS.txt", nullptr, nullptr, nullptr, "Image2D_FS.txt" },
		FShader::FCodePtrArray{ nullptr, "Text2D_VS.txt", nullptr, nullptr, nullptr, "Text2D_FS.txt" }
	};

	const TArray<const FChar* const, ArraySize> ShaderNames =
	{
		EShaderName::ModelDeferred,
		EShaderName::ModelDeferredFinal,
		EShaderName::Image2D,
		EShaderName::Text2D
	};

	const TArray<THandle<FShader>* const, ArraySize> ShaderHandles =
	{
		&Eng.MDShader,
		&Eng.MDFShader,
		&Eng.I2DShader,
		&Eng.T2DShader
	};

	for (SizeT I = 0; I < ArraySize; ++I)
	{
		THandle<FShader>& ShaderHandle = *ShaderHandles[I];
		ShaderHandle = FGFXHelper::LoadAndCacheShader(
			*Eng.Caches,
			*Eng.Handles,
			CodeArray[I],
			RelativeDirPath,
			ShaderNames[I]);

		if (!ShaderHandle.IsValid())
		{
			F_GFXLogError("Failed to load and cache shader.");
			F_GL(GL::BindVertexArray(0));
			F_GL(GL::DeleteVertexArrays(1, &VertexArray));
			return false;
		}
	}

	F_GL(GL::BindVertexArray(0));
	F_GL(GL::DeleteVertexArrays(1, &VertexArray));
	return true;
}

void FGFXEngine::ThreadDrawScene()
{
	F_Assert(IsValid(), "This class is invalid.");
	auto& Eng = Get();
	auto& RenderData = *Eng.RenderData;

	if (!RenderData.IsReadyForRender)
	{
		return;
	}

	RenderData.IsReadyForRender = false;

	// #FIXME: Debug only.
#if 0
	{
		static bool bOnceFlag = true;

		if (bOnceFlag)
		{
			bOnceFlag = false;
			for (SizeT I = 1; I < 252; ++I)
			{
				std::string Name = "golem - Copy (" + std::to_string(I) + ").jpg";

				FImageInstance Img;
				Img.SetImage(std::move(Name));

				RenderData.Images.push_back(Img);
			}
		}
	}
#endif

	const EClearBit::Type EGLClearBits = EClearBit::Color | EClearBit::Depth | EClearBit::Stencil;
	F_GLDisplayErrors();
	F_GL(GL::Clear(EGLClearBits));

	const FCamera& PersCam = RenderData.PersCam;
	const FCamera& OrthoCam = RenderData.OrthoCam;

	const FMatrix4D& ProjectionMatrix = PersCam.GetProjection();
	const FMatrix4D& OrthoMatrix = OrthoCam.GetProjection();
	const FMatrix4D& ViewMatrix = PersCam.GetView();
	const FMatrix4D ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

#pragma region Process Completed Tasks

	{
		if (Eng.Tasks.empty())
		{
			Eng.MsgReceiver->RetrieveTasks(Eng.Tasks);
		}

		if (!Eng.Tasks.empty())
		{
			FGFXTaskReceiver::FTask& Task = Eng.Tasks.back();
			if (Task())
			{
				Eng.Tasks.pop_back();
			}
			else
			{
				F_LogError("Task failed.");
				Eng.Tasks.clear();
			}
		}
	}

#pragma endregion

#pragma region Model Rendering Set Up

	FModelRenderList RenderedModels;
	FAsyncTasks AsyncTasks;
	TVector<FString> AsyncTaskNames;

	{
		const FModelCache& ModelCache = Eng.Caches->GetModelCache();

		for (const auto& Model : RenderData.Models)
		{
			const auto& ModelName = Model.GetModel();
			THandle<FModel> ModelHandle;
			ModelHandle = ModelCache.GetItem(ModelName, ModelHandle);

			if (ModelHandle.IsValid())
			{
				F_Assert(ModelHandle.GetRefCount() > 1, "Model handle should have a ref count of 2 or more.");
				RenderedModels.emplace_back(std::make_pair(&Model, std::move(ModelHandle)));
				continue;
			}

#pragma region Model Async Load
			auto Iter = Eng.PendingAsyncTasks.find(ModelName);
			if (Iter == Eng.PendingAsyncTasks.end())
			{
				AsyncTasks.emplace_back([&Eng, ModelName]()
				{
					FGFXAsyncTasks::ProcessModel(Eng, ModelName);
				});

				AsyncTaskNames.emplace_back(std::move(ModelName));
			}
#pragma endregion
		}
	}

#pragma endregion

#pragma region Image Rendering Set Up

	FImageRenderList RenderedImages;

	{
		const FImageCache& ImageCache = Eng.Caches->GetImageCache();

		for (const auto& Image : RenderData.Images)
		{
			const auto& ImageName = Image.GetImage();
			THandle<FImage> ImageHandle;
			ImageHandle = ImageCache.GetItem(ImageName, ImageHandle);

			if (ImageHandle.IsValid())
			{
				F_Assert(ImageHandle.GetRefCount() > 1, "Image handle should have a ref count of 2 or more.");
				RenderedImages.emplace_back(std::make_pair(&Image, std::move(ImageHandle)));
				continue;
			}

#pragma region Image Async Load
			auto Iter = Eng.PendingAsyncTasks.find(ImageName);
			if (Iter == Eng.PendingAsyncTasks.end())
			{
				AsyncTasks.emplace_back([&Eng, ImageName]()
				{
					FGFXAsyncTasks::ProcessImage(Eng, ImageName);
				});

				AsyncTaskNames.emplace_back(std::move(ImageName));
			}
#pragma endregion
		}

		auto CompareFunc = [](const FImageRenderEntry& LHS, const FImageRenderEntry& RHS)
		{
			return LHS.first->GetLayer() > RHS.first->GetLayer();
		};

		std::sort(RenderedImages.begin(), RenderedImages.end(), CompareFunc);
	}

#pragma endregion

#pragma region Dispatch Tasks

	{
		for (const auto& AsyncTaskName : AsyncTaskNames)
		{
			Eng.PendingAsyncTasks.emplace(std::move(AsyncTaskName));
		}

		Eng.TaskHandler->AddAsyncTasks(std::move(AsyncTasks));
	}

#pragma endregion

#pragma region Model Rendering
	{
		Eng.GBuffer.EnableWrite();
		F_GL(GL::Clear(EGLClearBits));
		F_GL(GL::ActiveTexture(ETex::T0));

		Eng.MDShader->Enable();
		Eng.MDShader->SetUniform(0, EUniform::DiffuseMap);

		FGFXHelper::RenderModels(Eng, RenderedModels, ViewProjectionMatrix);

		Eng.GBuffer.DisableWrite();
		Eng.GBuffer.EnableRead();

		Eng.MDFShader->Enable();
		Eng.MDFShader->SetUniform(0, EUniform::DiffuseMap);

		for (EGBufferTex::Type I = 0; I < EGBufferTex::Count; ++I)
		{
			const ETex::Value ActiveTex = static_cast<ETex::Value>(ETex::T0 + I);
			const EGBufferTex::Value BufferTex = static_cast<EGBufferTex::Value>(I);

			F_GL(GL::ActiveTexture(ActiveTex));
			Eng.GBuffer.BindTexture(BufferTex);
		}

		F_GL(GL::Disable(ECapability::DepthTest));
		F_GL(GL::BindVertexArray(Eng.MeshQuad.GetVertexArray()));
		F_GL(GL::DrawArrays(EMode::Triangles, 0, Eng.MeshQuad.GetVertexCount()));

		Eng.GBuffer.DisableRead();
	}

#pragma endregion

#pragma region Image Rendering
	{
		F_GL(GL::ActiveTexture(ETex::T0));
		F_GL(GL::Enable(ECapability::Blend));
		F_GL(GL::BlendFunc(EBlend::SrcAlpha, EBlend::OneMinusSrcAlpha));

		Eng.I2DShader->Enable();
		Eng.I2DShader->SetUniform(OrthoMatrix, EUniform::Transform4D);
		Eng.I2DShader->SetUniform(0, EUniform::DiffuseMap);

		const GL::LocationT ColorLoc = Eng.I2DShader->GetUniform(EUniform::Color4D);
		const GL::LocationT PositionLoc = Eng.I2DShader->GetUniform(EUniform::Position2D);
		const GL::LocationT ScaleLoc = Eng.I2DShader->GetUniform(EUniform::Scale2D);
		const GL::LocationT FlipLoc = Eng.I2DShader->GetUniform(EUniform::Flip2D);
		const GL::LocationT ViewRectLoc = Eng.I2DShader->GetUniform(EUniform::ViewRect2D);

		GL::ImageIDT LastBoundImageID = 0;

		for (SizeT I = 0, RISize = RenderedImages.size(); I < RISize; ++I)
		{
			const FImageInstance& I2DInstance = *RenderedImages[I].first;
			const FImage& ImageResource = RenderedImages[I].second.Get();

			const FVector4D& Color = I2DInstance.GetColor();
			const FVector2D& Position = I2DInstance.GetPosition();
			const FVector2D Scale = I2DInstance.GetScale() * ImageResource.GetDimensions();
			const FVector2D Flip(I2DInstance.GetFlip().x, -I2DInstance.GetFlip().y);
			const FVector4D& ViewRect = I2DInstance.GetViewRectangle();

			Eng.I2DShader->SetUniform(Color, ColorLoc);
			Eng.I2DShader->SetUniform(Position, PositionLoc);
			Eng.I2DShader->SetUniform(Scale, ScaleLoc);
			Eng.I2DShader->SetUniform(Flip, FlipLoc);
			Eng.I2DShader->SetUniform(ViewRect, ViewRectLoc);

			const GL::ImageIDT CurrentImageID = ImageResource.GetID();
			if (LastBoundImageID != CurrentImageID)
			{
				LastBoundImageID = CurrentImageID;
				ImageResource.Enable();
			}

			F_GL(GL::DrawArrays(EMode::Triangles, 0, Eng.MeshQuad.GetVertexCount()));
		}
	}

#pragma endregion

#pragma region Text Rendering
	{
		F_GL(GL::PixelStorei(EPixelStorage::UnpackAlignment, 1));

		// #FIXME...

		F_GL(GL::Disable(ECapability::Blend));
		F_GL(GL::Enable(ECapability::DepthTest));
		F_GL(GL::PixelStorei(EPixelStorage::UnpackAlignment, 4));
	}
#pragma endregion

	Eng.InitParams.Window->BufferSwap();
	F_GLDisplayErrors();

#if PHOENIX_GFX_MANAGES_A_PROFILER
	if (F_GFXGetProfiler().HasEntries())
	{
#	if PHOENIX_GFX_ENABLE_PROFILER_LOGGING_IN_MT_MODE
		F_GFXLog(F_GFXGetProfiler());
#	endif
		F_GFXResetProfiler();
	}
#endif
}

bool FGFXHelper::InitScene(FGFXScene& Scene, const FGFXEngine::FInitParams& InitParams)
{
	Scene.Init();
	F_Assert(Scene.IsValid(), "GFXScene failed to initialize.");

	{
		const Float32 Width = InitParams.Window->GetDimensions().x;
		const Float32 Height = InitParams.Window->GetDimensions().y;

		const Float32 HalfWidth = Width * 0.5f;
		const Float32 HalfHeight = Height * 0.5f;

		const Float32 FOV = glm::radians(45.f);
		const Float32 AspectRatio = Width / Height;
		const Float32 Near = 0.1f;
		const Float32 Far = 1000.f;

		const FVector3D Position = FVector3D(0, 0, 5.f);
		const FVector3D LookAtTarget = FVector3D(0, 0, 0);
		const FVector3D UpVector = FVector3D(0, 1, 0);

		{
			auto& PersCam = Scene.GetPerspectiveCamera();
			PersCam->SetPerspective(FOV, AspectRatio, Near, Far);
			PersCam->SetView(Position, LookAtTarget, UpVector);
		}

		{
			auto& OrthoCam = Scene.GetOrthographicCamera();
			OrthoCam->SetOrthographic(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight);
		}
	}

	const bool bIsValid = Scene.IsValid();
	return bIsValid;
}

THandle<FImage> FGFXHelper::LoadAndCacheImage(
	FGFXCaches& GFXCaches,
	FGFXHandles& GFXHandles,
	FString&& ImageFileName,
	const FChar* const RelativeDirPath,
	const EPixelFormat::Value PixelFormat)
{
	FImageCache& ImageCache = GFXCaches.GetImageCache();
	THandle<FImage> Image = ImageCache.GetItemCopy(ImageFileName);

	if (Image.IsValid())
	{
		F_Assert(Image->IsValid(), "A Image was cached but the resource it points to is invalid.");
		return Image;
	}

	const FString ImageAssetPath = RelativeDirPath + ImageFileName;

	FImageProcessor::FLoadParams LoadParams(ImageAssetPath.c_str(), PixelFormat);
	FImageProcessor ImageProcessor;
	ImageProcessor.Load(LoadParams);

	FImageHandles& ImageHandles = GFXHandles.GetImageHandles();

	Image = LoadAndCacheImageGL(ImageCache, ImageHandles, std::move(ImageFileName), ImageProcessor);
	return Image;
}

THandle<FImage> FGFXHelper::LoadAndCacheImageGL(
	FImageCache& ImageCache,
	FImageHandles& ImageHandles,
	FString&& ImageFileName,
	const FImageProcessor& ImageProcessor)
{
	THandle<FImage> Image;

	if (!ImageProcessor.IsValid())
	{
		F_GFXLogError("Image processor is invalid.");
		return Image;
	}

	FImage::FInitParams InitParams;
	InitParams.ImageData = &ImageProcessor.GetImageData();

	Image = ImageHandles.CreateHandle();
	Image->Init(InitParams);

	if (!Image->IsValid())
	{
		F_GFXLogError("Image handle is invalid.");
		Image.DeInit();
		return Image;
	}

	ImageCache.AddEntry(std::move(ImageFileName), THandle<FImage>(Image));
	return Image;
}

THandle<FModel> FGFXHelper::LoadAndCacheModel(
	FGFXCaches& GFXCaches,
	FGFXHandles& GFXHandles,
	FString&& ModelFileName,
	const FChar* const RelativeDirPath,
	const EMeshAttribute::Type MeshAttributes)
{
	FModelCache& ModelCache = GFXCaches.GetModelCache();
	THandle<FModel> Model = ModelCache.GetItemCopy(ModelFileName);

	if (Model.IsValid())
	{
		F_Assert(Model->IsValid(), "A model was cached but the resource it points to is invalid.");
		return Model;
	}

	const FString ModelAssetPath = RelativeDirPath + ModelFileName;

	FModelProcessor::FLoadParams LoadParams(ModelAssetPath.c_str(), MeshAttributes);
	FModelProcessor ModelProcessor;
	ModelProcessor.Load(LoadParams);

	Model = LoadAndCacheModelGL(GFXCaches, GFXHandles, std::move(ModelFileName), ModelProcessor);
	return Model;
}

THandle<FModel> FGFXHelper::LoadAndCacheModelGL(
	FGFXCaches& GFXCaches,
	FGFXHandles& GFXHandles,
	FString&& ModelFileName,
	const FModelProcessor& ModelProcessor)
{
	THandle<FModel> Model;
	if (!ModelProcessor.IsValid())
	{
		F_GFXLogError("Model processor is missing its mesh data entries.");
		return Model;
	}

	const FChar* const RelativeTexturesPath = EAssetPath::Get(EAssetPath::Textures);
	FString TexFileName;

	for (const auto& Mesh : ModelProcessor.GetMeshData())
	{
		static_assert(EMeshDataIndex::Count == 2, "This section requires updating.");
		for (EMeshDataIndex::Type I = 0; I < EMeshDataIndex::Count; ++I)
		{
			const EMeshDataIndex::Value MeshDataIndex = static_cast<EMeshDataIndex::Value>(I);
			TexFileName = Mesh.GetTextureName(MeshDataIndex);
			THandle<FImage> Image = FGFXHelper::LoadAndCacheImage(
				GFXCaches,
				GFXHandles,
				std::move(TexFileName),
				RelativeTexturesPath,
				EPixelFormat::RGB);

			if (!Image.IsValid())
			{
				F_GFXLogError("Failed to cache image { " << TexFileName << " }");
				return Model;
			}
		}
	}

	FModelHandles& ModelHandles = GFXHandles.GetModelHandles();
	Model = ModelHandles.CreateHandle();
	Model->Init(ModelProcessor.GetMeshData(), GFXCaches.GetImageCache());

	if (!Model->IsValid())
	{
		F_GFXLogError("Model is invalid.");
		Model.DeInit();
		return Model;
	}

	FModelCache& ModelCache = GFXCaches.GetModelCache();
	ModelCache.AddEntry(std::move(ModelFileName), THandle<FModel>(Model));
	return Model;
}

THandle<FShader> FGFXHelper::LoadAndCacheShader(
	FGFXCaches& GFXCaches,
	FGFXHandles& GFXHandles,
	const FShader::FCodePtrArray& ShaderFileNames,
	const FChar* const RelativeDirPath,
	const FChar* const ShaderKey)
{
	F_Assert(RelativeDirPath, "RelativeDirPath is null.");
	F_Assert(ShaderKey, "ShaderKey is null.");

	FString ShaderKeyStr = ShaderKey;

	FShaderCache& ShaderCache = GFXCaches.GetShaderCache();
	THandle<FShader> Shader = ShaderCache.GetItemCopy(ShaderKeyStr);

	if (Shader.IsValid())
	{
		F_Assert(Shader->IsValid(), "A Shader was cached but the resource it points to is invalid.");
		return Shader;
	}

	FString ShaderAssetPath;
	FString RelativeDirPathStr = RelativeDirPath;

	FShader::FCodeStrArray CodeStrArray;
	FShader::FInitParams InitParams;

	for (UInt32 I = 0; I < ShaderFileNames.size(); ++I)
	{
		if (!ShaderFileNames[I])
		{
			continue;
		}

		ShaderAssetPath = RelativeDirPathStr;
		ShaderAssetPath += ShaderFileNames[I];

		F_Assert(ShaderAssetPath.size(), "Invalid file path.");

		FFile File(ShaderAssetPath);

		if (File.Empty())
		{
			F_GFXLogError("Failed to open file " << ShaderAssetPath);
			return Shader;
		}

		CodeStrArray[I] = std::move(File.Content());
		InitParams.CodePtrArray[I] = CodeStrArray[I].c_str();

		F_Assert(CodeStrArray[I].size(), "Invalid shader file " << ShaderAssetPath);
	}

	FShaderHandles& ShaderHandles = GFXHandles.GetShaderHandles();
	Shader = ShaderHandles.CreateHandle();
	Shader->Init(InitParams);

	if (!Shader->IsValid())
	{
		F_GFXLogError("Shader failed to initialize.  Key { " << ShaderKey << " }");
		Shader.DeInit();
		return Shader;
	}

	ShaderCache.AddEntry(std::move(ShaderKeyStr), THandle<FShader>(Shader));
	return Shader;
}

void FGFXHelper::RenderModels(
	const FGFXEngineInternals& Eng,
	const FModelRenderList& ModelRenderList,
	const FMatrix4D& ViewProjectionMatrix)
{
	const GL::LocationT WVPMatrixLoc = Eng.MDShader->GetUniform(EUniform::WorldViewProjection4D);
	const GL::LocationT OriginLoc = Eng.MDShader->GetUniform(EUniform::Origin3D);

	for (const auto& RenderEntry : ModelRenderList)
	{
		const FModelInstance& ModelInstance = *RenderEntry.first;
		const FModel& Model = RenderEntry.second.Get();

		const FVector3D& Origin = ModelInstance.GetOrigin();
		const FVector3D& Position = ModelInstance.GetPosition();
		const FVector3D& Scale = ModelInstance.GetScale();
		const FQuaternion& Rotation = ModelInstance.GetRotation();

		const FMatrix4D WorldMatrix = NMatrix4D::Create(Position, Scale, Rotation);
		const FMatrix4D WVPMatrix = ViewProjectionMatrix * WorldMatrix;
		//FMatrix3D ITWorldMatrix = FMatrix3D(glm::transpose(glm::inverse(WorldMatrix)));

		Eng.MDShader->SetUniform(WVPMatrix, WVPMatrixLoc);
		Eng.MDShader->SetUniform(Origin, OriginLoc);

		for (const auto& Mesh : Model.GetMeshes())
		{
			Mesh.GetDiffuseImage()->Enable();
			F_Assert(Mesh.IsValid(), "Mesh vertex array is invalid.");
			F_GL(GL::BindVertexArray(Mesh.GetVertexArray()));
			F_GL(GL::DrawElements(EMode::Triangles, Mesh.GetIndexCount(), Mesh.GetIndexType(), nullptr));
		}
	}
}

void FGFXAsyncTasks::ProcessModel(FGFXEngineInternals& Eng, const FString& ModelName)
{
	const FString ModelAssetPath = EAssetPath::Get(EAssetPath::Models) + ModelName;

	FModelProcessor::FLoadParams LoadParams(ModelAssetPath.c_str(), EMeshAttribute::All);
	TAssertOnCopy<FModelProcessor> ModelProcessor;
	ModelProcessor->Load(LoadParams);

	if (!ModelProcessor->IsValid())
	{
		F_GFXLogError("Failed async load: { " << ModelAssetPath << " }");
		return;
	}

	TUnorderedMap<FString, TAssertOnCopy<FImageProcessor>> ImageProcessors;

	FString ImageName;
	FString ImageAssetPath;

	for (const auto& Mesh : ModelProcessor->GetMeshData())
	{
		static_assert(EMeshDataIndex::Count == 2, "This section requires updating.");
		for (EMeshDataIndex::Type I = 0; I < EMeshDataIndex::Count; ++I)
		{
			const EMeshDataIndex::Value MeshDataIndex = static_cast<EMeshDataIndex::Value>(I);
			if (!Mesh.HasTextureName(MeshDataIndex))
			{
				continue;
			}

			ImageName = Mesh.GetTextureName(MeshDataIndex);

			const auto Iter = ImageProcessors.find(ImageName);
			if (Iter == ImageProcessors.end())
			{
				ImageAssetPath = EAssetPath::Get(EAssetPath::Textures) + ImageName;

				FImageProcessor::FLoadParams LoadParams(ImageAssetPath.c_str(), EPixelFormat::RGB);
				TAssertOnCopy<FImageProcessor> ImageProcessor;
				ImageProcessor->Load(LoadParams);

				if (!ImageProcessor->IsValid())
				{
					F_GFXLogError("Failed async load: { " << ImageAssetPath << " }");
					return;
				}

				ImageProcessors.emplace(std::move(ImageName), std::move(ImageProcessor));
				ImageName = std::move(ImageAssetPath);
			}
		}
	}

	const SizeT GFXThreadTasksSize = 2 + ImageProcessors.size();

	FGFXTaskReceiver::FTasks GFXThreadTasks;
	GFXThreadTasks.reserve(GFXThreadTasksSize);

	GFXThreadTasks.emplace_back([&Eng, ModelName]()
	{
		Eng.PendingAsyncTasks.erase(ModelName);
		return true;
	});

	GFXThreadTasks.emplace_back([&Eng, ModelName, MdlProcessor = std::move(ModelProcessor)]()
	{
		const FModelCache& ModelCache = Eng.Caches->GetModelCache();
		if (ModelCache.HasItem(ModelName))
		{
			F_LogError("{ " << ModelName << " } has already been cached.");
			return true;
		}

		THandle<FModel> Model = FGFXHelper::LoadAndCacheModelGL(
			*Eng.Caches,
			*Eng.Handles,
			FString(ModelName),
			*MdlProcessor);

		if (!Model->IsValid())
		{
			F_LogError("Failed to process { " << ModelName << " }");
			return false;
		}

		return true;
	});

	for (auto& ImageProcessor : ImageProcessors)
	{
		GFXThreadTasks.emplace_back([&Eng, ImgName = ImageProcessor.first, ImgProcessor = std::move(ImageProcessor.second)]()
		{
			const FImageCache& ImageCache = Eng.Caches->GetImageCache();
			if (ImageCache.HasItem(ImgName))
			{
				F_LogError("{ " << ImgName << " } has already been cached.");
				return true;
			}

			THandle<FImage> Image = FGFXHelper::LoadAndCacheImageGL(
				Eng.Caches->GetImageCache(),
				Eng.Handles->GetImageHandles(),
				FString(ImgName),
				*ImgProcessor);

			if (!Image->IsValid())
			{
				F_LogError("Failed to process { " << ImgName << " }");
				return false;
			}

			return true;
		});
	}

	Eng.MsgReceiver->ReceiveTasks(std::move(GFXThreadTasks));
}

void FGFXAsyncTasks::ProcessImage(FGFXEngineInternals& Eng, const FString& ImageName)
{
	const FString ImageAssetPath = EAssetPath::Get(EAssetPath::Textures) + ImageName;

	FImageProcessor::FLoadParams LoadParams(ImageAssetPath.c_str(), EPixelFormat::RGBA);
	TAssertOnCopy<FImageProcessor> ImageProcessor;
	ImageProcessor->Load(LoadParams);

	if (!ImageProcessor->IsValid())
	{
		F_GFXLogError("Failed async load: { " << ImageAssetPath << " }");
		return;
	}

	const SizeT GFXThreadTasksSize = 2;

	FGFXTaskReceiver::FTasks GFXThreadTasks;
	GFXThreadTasks.reserve(GFXThreadTasksSize);

	GFXThreadTasks.emplace_back([&Eng, ImageName]()
	{
		Eng.PendingAsyncTasks.erase(ImageName);
		return true;
	});

	GFXThreadTasks.emplace_back([&Eng, ImageName, ImgProcessor = std::move(ImageProcessor)]()
	{
		const FImageCache& ImageCache = Eng.Caches->GetImageCache();
		if (ImageCache.HasItem(ImageName))
		{
			F_LogError("{ " << ImageName << " } has already been cached.");
			return true;
		}

		THandle<FImage> Image = FGFXHelper::LoadAndCacheImageGL(
			Eng.Caches->GetImageCache(),
			Eng.Handles->GetImageHandles(),
			FString(ImageName),
			*ImgProcessor);

		if (!Image->IsValid())
		{
			F_LogError("Failed to process { " << ImageName << " }");
			return false;
		}

		return true;
	});

	Eng.MsgReceiver->ReceiveTasks(std::move(GFXThreadTasks));
}
