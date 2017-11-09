#pragma once
#ifndef PHOENIX_COMPONENT_MANAGER_H
#define PHOENIX_COMPONENT_MANAGER_H

#include "ECS/ComponentStorage.h"
#include "ECS/Entity.h"
#include "ECS/RequirementBitArrayStorage.h"
#include "ECS/SystemStorage.h"
#include "Platform/Event/Event.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Debug/Assert.h"
#include "Utility/Misc/Primitives.h"

//Debug
#include <iostream>

namespace Phoenix
{
	template<typename TConfig>
	class TComponentManager
	{
	public:
		typedef SizeT EntityID;

		TComponentManager();

		//Interface for accessing Entities
		bool IsAlive(EntityID ID) const;
		EntityID CreateEntity();
		void Destroy(EntityID ID);

		//Component Operations
		template<typename TComponent>
		bool HasComponent(EntityID ID) const;

		template<typename TComponent>
		const TComponent& GetComponent(EntityID ID) const;

		template<typename TComponent>
		TComponent& GetComponent(EntityID ID);

		template<typename TComponent, typename... TArgs>
		TComponent& AddComponent(EntityID ID, TArgs&&... Args);

		template<typename TComponent>
		void RemoveComponent(EntityID ID);

		//Tag Operations
		template<typename TTag>
		bool HasTag(EntityID ID) const;

		template<typename TTag>
		void AddTag(EntityID ID);

		template<typename TTag>
		void RemoveTag(EntityID ID);

		template<typename TSystem>
		TSystem& GetSystem();

		//Requirements
		template<typename TRequirement>
		bool MeetsRequirement(EntityID ID) const;

		/*! \brief Run the provided function for all Entities. Function should take an EntityID
		*/
		template<typename TFunc>
		void ForEntities(TFunc&& Func);
		
		/*! \brief Run the provided function for all Entities that meet the requirement.
		*	\ The Function should take an EntityID, and references to the required Components
		*/
		template<typename TRequirement, typename TFunc>
		void ForEntitiesMeetingRequirement(TFunc&& Func);

		/*! \brief Reorder Entities: Move Alive towards the beginning and Dead towards the end.
		 *	\ Should ideally be called near the end of a frame (after all systems have updated)
		*/
		void Refresh();

		SizeT GetEntityCount() const;
		SizeT GetCapacity() const;

		/*! \brief Maintains the capacity, just resets all the entities and sets size down to 0
		*/
		void Clear();
		
		//Interface to call methods on all Systems
		void InitSystems();
		void DeInitSystems();
		void UpdateSystems(const FUpdateEvent& UpdateEvent);
		void NotifySystemsOnEvent(const FEvent& Event);

	private:
		void NotifySystemsEntitesCreated();
		void NotifySystemsEntityDestroyed(EntityID DestroyedEntity);

	private:
		friend class FECSTest;

		static const SizeT StartingSize = 10;

		using TComponentsBitArray = typename TConfig::ComponentsBitArray;
		using FEntity = TEntity<TComponentsBitArray>;
		using FRequirementBitArrayStorage = TRequirementBitArrayStorage<TConfig>;
		using FComponentStorage = TComponentStorage<TConfig>;
		using TSystemList = typename TConfig::SystemList;
		using FSystemStorage = TSystemStorage<TSystemList>;

		SizeT Capacity { 0 }; //Will need to resize if Capacity is exceeded
		SizeT Size { 0 }; //Current number of active entities (including dead ones, but not newly created ones)
		SizeT FirstUnusedEntityID { 0 }; //Predicted Size after new entities are created

		TVector<FEntity> Entities;
		TVector<EntityID> NewEntityList;

		FRequirementBitArrayStorage RequirementBitArrays; //BitArrays corresponding to each Systems' Requirements
		FComponentStorage ComponentStorage;
		FSystemStorage SystemStorage;

		void Resize(SizeT NewCapacity);
		void ResizeIfNeeded();

		FEntity& GetEntityByID(EntityID ID);
		const FEntity& GetEntityByID(EntityID ID) const;

		EntityID RefreshImpl();
		
		//Debug
		void PrintState() const;

		template<typename... TRequiredComponents>
		struct ForEntitiesHelper
		{
			template<typename TFunc>
			static void Call(FComponentStorage& ComponentStorage, EntityID ID, SizeT ComponentArrayIndex, TFunc&& Func)
			{
				//expands to: Func(ID, CTransform&, CRigidbody&); for example
				Func(ID, ComponentStorage.template GetComponent<TRequiredComponents>(ComponentArrayIndex)...);
			}
		};
	};

	//////////////////
	//Implementation
	//////////////////

	template<typename TConfig>
	TComponentManager<TConfig>::TComponentManager()
	{
		Resize(StartingSize);
	}

	template<typename TConfig>
	bool TComponentManager<TConfig>::IsAlive(EntityID ID) const
	{
		const FEntity& Entity = GetEntityByID(ID);
		return Entity.Alive;
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::Destroy(EntityID ID)
	{
		NotifySystemsEntityDestroyed(ID);

		FEntity& Entity = GetEntityByID(ID);
		Entity.Alive = false;
	}

	template<typename TConfig>
	typename TComponentManager<TConfig>::EntityID 
		TComponentManager<TConfig>::CreateEntity()
	{
		ResizeIfNeeded();

		//Predicted Size marks the end of the active and pending Entities
		EntityID NewEntityID = FirstUnusedEntityID++;

		F_AssertFalse(IsAlive(NewEntityID), "New Entity should be dead to begin with");

		//Initialize Entity
		FEntity& NewEntity = GetEntityByID(NewEntityID);
		NewEntity.Alive = true;
		NewEntity.BitArray.reset();

		NewEntityList.push_back(NewEntityID);

		return NewEntityID;
	}

	template<typename TConfig>
	template<typename TComponent>
	bool TComponentManager<TConfig>::HasComponent(EntityID ID) const
	{
		static_assert(TConfig::template IsComponent<TComponent>(), "Not a Component");

		const SizeT ComponentID = TConfig::template GetComponentBit<TComponent>();
		const FEntity& Entity = GetEntityByID(ID);

		const bool EntityHasComponent = Entity.BitArray[ComponentID];
		return EntityHasComponent;
	}

	template<typename TConfig>
	template<typename TComponent>
	const TComponent& TComponentManager<TConfig>::GetComponent(EntityID ID) const
	{
		static_assert(TConfig::template IsComponent<TComponent>(), "Not a Component");
		F_Assert(HasComponent<TComponent>(ID), "Entity does not have the Component");

		const FEntity& Entity = GetEntityByID(ID);
		const TComponent& Component = ComponentStorage.template GetComponent<TComponent>(Entity.ComponentArrayIndex);
		return Component;
	}

	template<typename TConfig>
	template<typename TComponent>
	TComponent& TComponentManager<TConfig>::GetComponent(EntityID ID)
	{
		static_assert(TConfig::template IsComponent<TComponent>(), "Not a Component");
		F_Assert(HasComponent<TComponent>(ID), "Entity does not have the Component");

		const FEntity& Entity = GetEntityByID(ID);
		TComponent& Component = ComponentStorage.template GetComponent<TComponent>(Entity.ComponentArrayIndex);
		return Component;
	}

	template<typename TConfig>
	template<typename TComponent, typename... TArgs>
	TComponent& TComponentManager<TConfig>::AddComponent(EntityID ID, TArgs&&... Args)
	{
		static_assert(TConfig::template IsComponent<TComponent>(), "Not a Component");

		FEntity& Entity = GetEntityByID(ID);
		const SizeT ComponentBit = TConfig::template GetComponentBit<TComponent>();
		Entity.BitArray[ComponentBit] = true;

		TComponent& Component = ComponentStorage.template GetComponent<TComponent>(Entity.ComponentArrayIndex);
		new (&Component) TComponent(std::forward<TArgs>(Args)...);

		return Component;
	}

	template<typename TConfig>
	template<typename TComponent>
	void TComponentManager<TConfig>::RemoveComponent(EntityID ID)
	{
		static_assert(TConfig::template IsComponent<TComponent>(), "Not a Component");

		const SizeT ComponentBit = TConfig::template GetComponentBit<TComponent>();
		FEntity& Entity = GetEntityByID(ID);

		Entity.BitArray[ComponentBit] = false;
	}

	template<typename TConfig>
	template<typename TTag>
	bool TComponentManager<TConfig>::HasTag(EntityID ID) const
	{
		static_assert(TConfig::template IsTag<TTag>(), "Not a Tag");

		const SizeT TagID = TConfig::template GetTagBit<TTag>();
		const FEntity& Entity = GetEntityByID(ID);

		const bool EntityHasTag = Entity.BitArray[TagID];
		return EntityHasTag;
	}

	template<typename TConfig>
	template<typename TTag>
	void TComponentManager<TConfig>::AddTag(EntityID ID)
	{
		static_assert(TConfig::template IsTag<TTag>(), "Not a Tag");

		const SizeT TagID = TConfig::template GetTagBit<TTag>();
		FEntity& Entity = GetEntityByID(ID);

		Entity.BitArray[TagID] = true;
	}

	template<typename TConfig>
	template<typename TTag>
	void TComponentManager<TConfig>::RemoveTag(EntityID ID)
	{
		static_assert(TConfig::template IsTag<TTag>(), "Not a Tag");

		const SizeT TagID = TConfig::template GetTagBit<TTag>();
		FEntity& Entity = GetEntityByID(ID);

		Entity.BitArray[TagID] = false;
	}

	template<typename TConfig>
	template<typename TSystem>
	TSystem& TComponentManager<TConfig>::TComponentManager::GetSystem()
	{
		return SystemStorage.template GetSystem<TSystem>();
	}

	template<typename TConfig>
	template<typename TRequirement>
	bool TComponentManager<TConfig>::MeetsRequirement(EntityID ID) const
	{
		static_assert(TConfig::template IsRequirement<TRequirement>(), "Not a Requirement");

		const FEntity& Entity = GetEntityByID(ID);
		const auto& EntityBitArray = Entity.BitArray;
		const auto& RequirementBitArray = RequirementBitArrays.template GetRequirementBitArray<TRequirement>();

		const bool Match = (RequirementBitArray & EntityBitArray) == RequirementBitArray;
		return Match;
	}
	
	template<typename TConfig>
	template<typename TFunc>
	void TComponentManager<TConfig>::ForEntities(TFunc&& Func)
	{
		for (EntityID ID = 0; ID < Size; ++ID)
		{
			Func(ID);
		}
	}

	template<typename TConfig>
	template<typename TRequirement, typename TFunc>
	void TComponentManager<TConfig>::ForEntitiesMeetingRequirement(TFunc&& Func)
	{
		static_assert(TConfig::template IsRequirement<TRequirement>(), "Not a Requirement");

		ForEntities([this, &Func](EntityID ID)
		{
			if (MeetsRequirement<TRequirement>(ID))
			{
				using RequiredComponents = typename FRequirementBitArrayStorage::template TComponentsInRequirement<TRequirement>;

				const FEntity& Entity = GetEntityByID(ID);
				const SizeT ComponentArrayIndex = Entity.ComponentArrayIndex;

				using TForEntitiesHelper = TRename<RequiredComponents, ForEntitiesHelper>;

				TForEntitiesHelper::Call(ComponentStorage, ID, ComponentArrayIndex, std::forward<TFunc>(Func));
			}
		});
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::Refresh()
	{
		//No Entities
		if (FirstUnusedEntityID == 0)
		{
			Size = 0;
			return;
		}

		Size = FirstUnusedEntityID = RefreshImpl();

		NotifySystemsEntitesCreated();
		NewEntityList.clear();
	}

	template<typename TConfig>
	typename TComponentManager<TConfig>::EntityID 
		TComponentManager<TConfig>::RefreshImpl()
	{
		EntityID LeftIndex { 0 }; //Will be looking for dead entities to move to the end
		EntityID RightIndex { FirstUnusedEntityID - 1 }; //Looking for alive entities at the end to move to the front

		while (true)
		{
			//Keep moving LeftIndex up until a dead entity is found
			while (true)
			{
				//Passing the right index means that there are no more dead entities.
				//The current index is the new size
				const bool PastRightIndex = LeftIndex > RightIndex;
				if (PastRightIndex)
				{
					return LeftIndex;
				}

				const bool DeadEntityFound = !IsAlive(LeftIndex);
				if (DeadEntityFound)
				{
					break;
				}
				++LeftIndex;
			}

			//move RightIndex down until the first alive entity is found
			while (true)
			{
				const bool AliveEntityFound = IsAlive(RightIndex);
				if (AliveEntityFound)
				{
					break;
				}

				//No more alive entities...this is the new size
				const bool CrossedLeftIndex = RightIndex <= LeftIndex;
				if(CrossedLeftIndex)
				{
					return RightIndex;
				}

				--RightIndex;
			}

			//Now we should have LeftIndex pointing towards a Dead entity and RightIndex at an Alive one
			F_AssertFalse(IsAlive(LeftIndex), "Left Index should point to a dead entity");
			F_Assert(IsAlive(RightIndex), "Right index should point to an Alive entity");

			//Swap the entities and update the indices to start looking again
			std::swap(Entities[LeftIndex], Entities[RightIndex]);

			//#TODO Figure out a better way to keep track of new entities
			auto NewEntityIter = std::find(NewEntityList.begin(), NewEntityList.end(), RightIndex);
			const bool EntitySwappedIsNewEntity = NewEntityIter != NewEntityList.end();
			if (EntitySwappedIsNewEntity)
			{
				*NewEntityIter = LeftIndex;
			}

			++LeftIndex;
			--RightIndex;
		}

		return LeftIndex;
	}

	template<typename TConfig>
	SizeT TComponentManager<TConfig>::GetEntityCount() const
	{
		return Size;
	}

	template<typename TConfig>
	SizeT TComponentManager<TConfig>::GetCapacity() const
	{
		return Capacity;
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::Clear()
	{
		for (SizeT I = 0; I < Capacity; ++I)
		{
			FEntity& Entity = Entities[I];
			Entity.Reset(I);
		}

		Size = FirstUnusedEntityID = 0;
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::InitSystems()
	{
		SystemStorage.Init(*this);
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::DeInitSystems()
	{
		SystemStorage.DeInit(*this);
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::UpdateSystems(const FUpdateEvent& UpdateEvent)
	{
		SystemStorage.Update(UpdateEvent, *this);
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::NotifySystemsOnEvent(const FEvent& Event)
	{
		SystemStorage.OnEvent(Event, *this);
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::NotifySystemsEntitesCreated()
	{
		SystemStorage.OnEntitiesCreated(NewEntityList, *this);
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::NotifySystemsEntityDestroyed(EntityID DestroyedEntity)
	{
		SystemStorage.OnEntityDestroyed(DestroyedEntity, *this);
	}



	template<typename TConfig>
	void TComponentManager<TConfig>::Resize(SizeT NewCapacity)
	{
		F_Assert(NewCapacity > Capacity, "New Capacity should be greater than current capacity");

		Entities.resize(NewCapacity);

		//Initialize the new entities
		for (SizeT I = Capacity; I < NewCapacity; ++I)
		{
			FEntity& Entity = Entities[I];
			Entity.Reset(I);
		}

		ComponentStorage.Resize(NewCapacity);

		Capacity = NewCapacity;
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::ResizeIfNeeded()
	{
		//No need to grow
		if (Capacity > FirstUnusedEntityID)
		{
			return;
		}

		//Resize to double capacity
		const SizeT NewCapacity = Capacity * 2;
		Resize(NewCapacity);
	}
	
	template<typename TConfig>
	typename TComponentManager<TConfig>::FEntity&
		TComponentManager<TConfig>::GetEntityByID(EntityID ID)
	{
		F_Assert(FirstUnusedEntityID > ID, "Entity at ID is not active");
		return Entities[ID];
	}

	template<typename TConfig>
	const typename TComponentManager<TConfig>::FEntity&
		TComponentManager<TConfig>::GetEntityByID(EntityID ID) const
	{
		F_Assert(FirstUnusedEntityID > ID, "Entity at ID is not active");
		return Entities[ID];
	}

	template<typename TConfig>
	void TComponentManager<TConfig>::PrintState() const
	{
		std::cout << "Size: " << Size << " || "
			<< "Predicted Size: " << FirstUnusedEntityID << " || "
			<< "Capacity: " << Capacity << "\n";

		for (SizeT I = 0; I < FirstUnusedEntityID; ++I)
		{
			std::cout << (IsAlive(I) ? '1' : '0');
		}
		std::cout << std::endl;
	}
}

#endif
