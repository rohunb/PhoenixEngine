#ifndef PHOENIX_GAME_SCENE_H
#define PHOENIX_GAME_SCENE_H

#include "Components/Core.h"
#include "Components/ComponentStorage.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectIdPool.h"
#include "Utility/Misc/Primitives.h"

#include "ECS/ComponentManager.h"
#include "Config/ECS/ComponentsRegistry.h"

namespace Phoenix
{
	class FGameScene
	{
		typedef std::vector<FGameObject> GameObjectArray;

		struct CoreDeleter
		{
			void operator() (BaseCore* InCore) const
			{
				InCore->GameScene = nullptr;
				InCore->Entities.clear();
			}
		};

		typedef std::unordered_map<TypeId, std::unique_ptr<BaseCore, CoreDeleter>> CoreArray;
	public:
		FGameScene();

		FGameScene(const FGameScene&) = delete;
		FGameScene& operator=(const FGameScene&) = delete;

		FGameScene(FGameScene&&) = delete;
		FGameScene& operator=(FGameScene&&) = delete;

		virtual ~FGameScene();

		typedef TComponentManager<FComponentRegistry::Config> FComponentManager;

		void Init(FComponentManager& ComponentManager);
		void Init(std::size_t InGameObjectPoolSize);

		void Update(const struct FUpdateEvent& UpdateEvent, FComponentManager& ComponentManager);

		void DeInit();

		template <typename TCore>
		void AddCore(TCore& inCore);

		FGameObject& CreateGameObject();

		std::size_t GetGameObjectCount() const;

	protected:
		virtual void OnInit(FComponentManager& ComponentManager) = 0;

		virtual void OnUpdate(const struct FUpdateEvent& UpdateEvent, FComponentManager& ComponentManager) = 0;

		virtual void OnDeInit() = 0;

	private:
		void SimulateGameObjects();

		CoreArray Cores;

		GameObjectIdPool ObjectIdPool;

		struct TGameObjectAttributes
		{
			struct Attribute
			{
				bool IsActive;

				std::vector<bool> Cores;
			};

			TGameObjectAttributes(std::size_t InGameObjectAmount) :
				Storage(InGameObjectAmount),
				Attributes(InGameObjectAmount)
			{

			}

			void Resize(std::size_t InAmount)
			{
				Storage.Resize(InAmount);
				Attributes.resize(InAmount);
			}

			ComponentStorage Storage;

			std::vector<Attribute> Attributes;
		}

		GameObjectAttributes;

		struct TGameObjectCache
		{
			GameObjectArray Alive;
			GameObjectArray Killed;
			GameObjectArray Activated;
			GameObjectArray Deactivated;

			void ClearTemp()
			{
				Killed.clear();
				Activated.clear();
				Deactivated.clear();
			}
		}

		GameObjectCache;

		void AddCore(BaseCore& InCore, TypeId InCoreTypeId);

		void CheckForResize(std::size_t InNumEntitiesToBeAllocated);

		void Resize(std::size_t InAmount);

		void ActivateGameObject(FGameObject& InGameObject, const bool InActive);

		// Access to components
		friend class FGameObject;
	};

	template<typename TCore>
	void FGameScene::AddCore(TCore& InCore)
	{
		AddCore(InCore, TCore::GetTypeId());
	}
}

#endif
