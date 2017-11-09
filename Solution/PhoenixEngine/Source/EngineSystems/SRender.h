#ifndef PHOENIX_S_RENDER_H
#define PHOENIX_S_RENDER_H

#include "Platform/Event/Event.h"
#include "Rendering/GFXScene.h"
#include "Utility/Misc/Memory.h"

namespace Phoenix
{
	template<typename TRequirement>
	class SRender
	{
	public:
		template<typename TComponentManager>
		void Init(TComponentManager& ComponentManager, FGFXScene& GFXScene);

		template<typename TComponentManager>
		void OnEntityCreated(SizeT NewEntity, TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void DeInit(TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void OnEntityDestroyed(SizeT DestroyedEntity, TComponentManager& ComponentManager);

	private:
		//#TODO Find a better solution to allow access to the GFXScene
		TRawPtr<FGFXScene> GFXScene;
	};


	template<typename TRequirement>
	template<typename TComponentManager>
	void SRender<TRequirement>::Init(TComponentManager& ComponentManager, FGFXScene& InGFXScene)
	{
		F_AssertFalse(GFXScene.IsValid(), "GFXScene should be null until Init");
		GFXScene = &InGFXScene;

		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([this](SizeT EntityID, CTransform& Transform, CModel& Model)
		{
			auto ModelInstance = GFXScene->CreateModel(Model.ModelFileName, FMaterial::CreateDefault());

			F_Assert(ModelInstance.IsValid(), "Model creation failed");

			ModelInstance->SetPosition(Transform.Position);
			ModelInstance->SetRotation(Transform.Rotation);
			ModelInstance->SetScale(Transform.Scale);

			Model.ModelInstance = ModelInstance;
		});
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SRender<TRequirement>::OnEntityCreated(SizeT NewEntity, TComponentManager& ComponentManager)
	{
		//Init hasn't run yet
		if (!GFXScene.IsValid())
		{
			return;
		}

		F_Assert(GFXScene->IsValid(), "GFXScene should be valid");

		const bool IsRenderable = ComponentManager.template HasComponent<CModel>(NewEntity)
			&& ComponentManager.template HasComponent<CTransform>(NewEntity);

		if (IsRenderable)
		{
			auto& Model = ComponentManager.template GetComponent<CModel>(NewEntity);
			const auto& Transform = ComponentManager.template GetComponent<CTransform>(NewEntity);

			auto ModelInstance = GFXScene->CreateModel(Model.ModelFileName, FMaterial::CreateDefault());
			F_Assert(ModelInstance.IsValid(), "Model creation failed");

			ModelInstance->SetPosition(Transform.Position);
			ModelInstance->SetRotation(Transform.Rotation);
			ModelInstance->SetScale(Transform.Scale);

			Model.ModelInstance = ModelInstance;
		}
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SRender<TRequirement>::Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
	{
		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([](SizeT EntityID, CTransform& Transform, CModel& Model)
		{
			Model.ModelInstance->SetPosition(Transform.Position);
			Model.ModelInstance->SetRotation(Transform.Rotation);
			Model.ModelInstance->SetScale(Transform.Scale);
		});
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SRender<TRequirement>::DeInit(TComponentManager& ComponentManager)
	{
		ComponentManager.template ForEntitiesMeetingRequirement<TRequirement>
			([](SizeT EntityID, CTransform&, CModel& Model)
		{
			Model.ModelInstance.DeInit();
		});
	}

	template<typename TRequirement>
	template<typename TComponentManager>
	void SRender<TRequirement>::OnEntityDestroyed(SizeT DestroyedEntity, TComponentManager& ComponentManager)
	{
		const bool IsRenderable = ComponentManager.template HasComponent<CModel>(DestroyedEntity)
			&& ComponentManager.template HasComponent<CTransform>(DestroyedEntity);

		if (IsRenderable)
		{
			CModel& Model = ComponentManager.template GetComponent<CModel>(DestroyedEntity);
			Model.ModelInstance.DeInit();
		}
	}
}

#endif
