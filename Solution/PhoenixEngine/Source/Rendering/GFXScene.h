#ifndef PHOENIX_GFX_SCENE_H
#define PHOENIX_GFX_SCENE_H

#include "Utility/Containers/Array.h"
#include "Utility/Handle/Handles.h"
#include "Utility/Misc/Memory.h"
#include "Utility/Misc/Primitives.h"
#include "Rendering/Camera.h"
#include "Rendering/GFXTypes.h"
#include "Rendering/Handles/CameraHandles.h"
#include "Rendering/Image/ImageInstance.h"
#include "Rendering/Model/ModelInstance.h"
#include "Rendering/Text/TextInstance.h"

namespace Phoenix
{
	namespace EImageInfo
	{
		typedef UInt32 Type;

		enum BitMask : Type
		{
			Default
		};
	}

	namespace EModelInfo
	{
		typedef UInt32 Type;

		enum BitMask : Type
		{
			Default
		};
	}

	typedef THandles<FImageInstance> FImageInstances;
	typedef THandles<FModelInstance> FModelInstances;
	typedef THandles<FTextInstance> FTextInstances;

	class FGFXScene
	{
	public:
		//	--------------------------------------------------------------------------------
		/*! \brief Default constructor. */
		FGFXScene() = default;

		//	--------------------------------------------------------------------------------
		/*! \brief This method cannot be used.
		*
		*	@note FGFXScene's memory location does not change once created.
		*/
		FGFXScene(const FGFXScene&) = delete;

		//	--------------------------------------------------------------------------------
		/*! \brief This method cannot be used.
		*
		*	@note FGFXScene's memory location does not change once created.
		*/
		FGFXScene& operator=(const FGFXScene&) = delete;

		//	--------------------------------------------------------------------------------
		/*! \brief This method cannot be used.
		*
		*	@note FGFXScene's memory location does not change once created.
		*/
		FGFXScene(FGFXScene&&) = delete;

		//	--------------------------------------------------------------------------------
		/*! \brief This method cannot be used.
		*
		*	@note FGFXScene's memory location does not change once created.
		*/
		FGFXScene& operator=(FGFXScene&&) = delete;

		//	--------------------------------------------------------------------------------
		/*! \brief Basic destructor. */
		~FGFXScene();

		//	--------------------------------------------------------------------------------
		/*! \brief Initializes FGFXScene. 
		*
		*	@note This is for engine use only.
		*/
		void Init();

		//	--------------------------------------------------------------------------------
		/*! \brief Determines if this class is in a usable state.
		*
		*	@return Returns true if this class is fully initialized, or false otherwise.
		*/
		bool IsValid() const;

		//	--------------------------------------------------------------------------------
		/*! \brief Deinitializes FGFXScene.
		*
		*	@note This is for engine use only.
		*/
		void DeInit();

		//	--------------------------------------------------------------------------------
		/*! \brief Sets the scene's 2D camera. */
		void SetOrthographicCamera(const THandle<FCamera>& Camera);

		//	--------------------------------------------------------------------------------
		/*! \brief Sets the scene's 3D camera. */
		void SetPerspectiveCamera(const THandle<FCamera>& Camera);

		//	--------------------------------------------------------------------------------
		/*! \brief Gets the scene's 2D camera.
		*
		*	@return Returns a handle to a FCamera.
		*	@note This handle is not guaranteed to be valid.
		*/
		THandle<FCamera>& GetOrthographicCamera();

		//	--------------------------------------------------------------------------------
		/*! \brief Gets the scene's 2D camera.
		*
		*	@return Returns a handle to a FCamera.
		*	@note This handle is not guaranteed to be valid.
		*/
		const THandle<FCamera>& GetOrthographicCamera() const;

		//	--------------------------------------------------------------------------------
		/*! \brief Gets the scene's 3D camera.
		*
		*	@return Returns a handle to a FCamera.
		*	@note This handle is not guaranteed to be valid.
		*/
		THandle<FCamera>& GetPerspectiveCamera();

		//	--------------------------------------------------------------------------------
		/*! \brief Gets the scene's 3D camera.
		*
		*	@return Returns a handle to a FCamera.
		*	@note This handle is not guaranteed to be valid.
		*/
		const THandle<FCamera>& GetPerspectiveCamera() const;

		//	--------------------------------------------------------------------------------
		/*! \brief Creates a Camera.
		*
		*	@return Returns a valid handle to a FCamera.
		*	@note This handle will always be valid.
		*/
		THandle<FCamera> CreateCamera();

		//	--------------------------------------------------------------------------------
		/*! \brief Creates a Image.
		*
		*	@param ImageFileName A string containing the name and extension of the model.
		*	@example "golemdiffuse.jpg"
		*	@return Returns a handle to a FImageInstance.  This handle will always be valid.
		*/
		THandle<FImageInstance> CreateImage(
			const FString& ImageFileName,
			const EImageInfo::Type ImageInfo = EImageInfo::Default);

		//	--------------------------------------------------------------------------------
		/*! \brief Creates a Model.
		*
		*	@param ModelFileName A string containing the name and extension of the model.
		*	@example "golem.pmesh"
		*	@return Returns a handle to a FModelInstance.  This handle will always be valid.
		*/
		THandle<FModelInstance> CreateModel(
			const FString& ModelFileName,
			const FMaterial& Material = FMaterial::CreateDefault(),
			const EModelInfo::Type ModelInfo = EModelInfo::Default);

		THandle<FTextInstance> CreateText(const FString& FontFileName);

		//	--------------------------------------------------------------------------------
		/*! \brief Gets all of the FImageInstances in the scene.
		*
		*	@return Returns all of the FImagesInstances in the scene.
		*/
		const FImageInstances& GetImageInstances() const;

		//	--------------------------------------------------------------------------------
		/*! \brief Gets all of the FModelInstances in the scene.
		*
		*	@return Returns all of the FModelInstances in the scene.
		*/
		const FModelInstances& GetModelInstances() const;

		//	--------------------------------------------------------------------------------
		/*! \brief Gets all of the FTextInstances in the scene.
		*
		*	@return Returns all of the FTextInstances in the scene.
		*/
		const FTextInstances& GetTextInstances() const;

	private:
		THandle<FCamera> PersCam;
		THandle<FCamera> OrthoCam;

		FImageInstances ImageInstances;
		FModelInstances ModelInstances;
		FTextInstances TextInstances;

		FCameraHandles CameraHandles;
	};
}

#endif
