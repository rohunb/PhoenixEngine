#ifndef PHOENIX_SYSTEM_STORAGE_H
#define PHOENIX_SYSTEM_STORAGE_H

#include "Platform/Event/Event.h"
#include "Utility/Containers/Tuple.h"
#include "Utility/Containers/Vector.h"
#include "Utility/MetaProgramming/For.h"
#include "Utility/MetaProgramming/Rename.h"
#include "Utility/MetaProgramming/HasMethod.h"

namespace Phoenix
{
	F_DefineTrait_HasMethod(Init);
	F_DefineTrait_HasMethod(DeInit);
	F_DefineTrait_HasMethod(Update);
	F_DefineTrait_HasMethod(OnEvent);
	F_DefineTrait_HasMethod(OnEntityCreated);
	F_DefineTrait_HasMethod(OnEntityDestroyed);

	template<typename TSystemList>
	class TSystemStorage
	{
	public:
		template<typename TSystem>
		TSystem& GetSystem();

		template<typename TSystem>
		const TSystem& GetSystem() const;

		template<typename TFunc>
		void ForSystems(TFunc&& Func);

		template<typename TComponentManager>
		void Init(TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void DeInit(TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void OnEvent(const FEvent& Event, TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void OnEntitiesCreated(const TVector<SizeT>& NewEntityList, TComponentManager& ComponentManager);

		template<typename TComponentManager>
		void OnEntityDestroyed(SizeT DestroyedEntity, TComponentManager& ComponentManager);

	private:
		using SystemList = TSystemList;

		using TTupleOfSystems = TRename<SystemList, TTuple>;

		TTupleOfSystems TupleOfSystems;

		template<typename TSystem>
		static constexpr bool IsSystem();

		//Has Init Method
		template<typename TSystem, typename TComponentManager
				, TEnableIf<THasMethod_Init<TSystem, TComponentManager>::Value, Int32> = 0>
		void CallInitIfDefined(TSystem& System, TComponentManager& ComponentManager);

		//No Init Method
		template<typename TSystem, typename TComponentManager
				, TDisableIf<THasMethod_Init<TSystem, TComponentManager>::Value, Int32> = 0>
		void CallInitIfDefined(TSystem&, TComponentManager&);

		//Has DeInit Method
		template<typename TSystem, typename TComponentManager
				, TEnableIf<THasMethod_DeInit<TSystem, TComponentManager>::Value, Int32> = 0>
		void CallDeInitIfDefined(TSystem& System, TComponentManager& ComponentManager);

		template<typename TSystem, typename TComponentManager
				, TDisableIf<THasMethod_DeInit<TSystem, TComponentManager>::Value, Int32> = 0>
		void CallDeInitIfDefined(TSystem&, TComponentManager&);

		//Has Update
		template<typename TSystem, typename TComponentManager
				, TEnableIf<THasMethod_Update<TSystem, FUpdateEvent, TComponentManager>::Value, Int32> = 0>
		void CallUpdateIfDefined(TSystem& System, const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager);

		//No Update
		template<typename TSystem, typename TComponentManager
				, TDisableIf<THasMethod_Update<TSystem, FUpdateEvent, TComponentManager>::Value, Int32> = 0>
		void CallUpdateIfDefined(TSystem&, const FUpdateEvent&, TComponentManager&);


		//Has OnEvent
		template<typename TSystem, typename TComponentManager
				, TEnableIf<THasMethod_OnEvent<TSystem, FEvent, TComponentManager>::Value, Int32> = 0>
		void CallOnEventIfDefined(TSystem& System, const FEvent& Event, TComponentManager& ComponentManager);

		//No OnEvent
		template<typename TSystem, typename TComponentManager
				, TDisableIf<THasMethod_OnEvent<TSystem, FEvent, TComponentManager>::Value, Int32> = 0>
		void CallOnEventIfDefined(TSystem&, const FEvent&, TComponentManager&);

		//EntityCreated
		template<typename TSystem, typename TComponentManager
				, TEnableIf<THasMethod_OnEntityCreated<TSystem, SizeT, TComponentManager>::Value, Int32> = 0>
		void CallOnEntityCreatedIfDefined(TSystem& System, SizeT NewEntity, TComponentManager& ComponentManager);

		template<typename TSystem, typename TComponentManager
				, TDisableIf<THasMethod_OnEntityCreated<TSystem, SizeT, TComponentManager>::Value, Int32> = 0>
		void CallOnEntityCreatedIfDefined(TSystem&, SizeT, TComponentManager&);

		//EntityDestroyed
		template<typename TSystem, typename TComponentManager
				, TEnableIf<THasMethod_OnEntityDestroyed<TSystem, SizeT, TComponentManager>::Value, Int32> = 0>
		void CallOnEntityDestroyedIfDefined(TSystem& System, SizeT EntityID, TComponentManager& ComponentManager);

		template<typename TSystem, typename TComponentManager
				, TDisableIf<THasMethod_OnEntityDestroyed<TSystem, SizeT, TComponentManager>::Value, Int32> = 0>
		void CallOnEntityDestroyedIfDefined(TSystem&, SizeT, TComponentManager&);
	};

	//Implementation
	template<typename TSystemList>
	template<typename TSystem>
	constexpr bool TSystemStorage<TSystemList>::IsSystem()
	{
		return TContains<TSystem, TSystemList>::value;
	}

	template<typename TConfig>
	template<typename TSystem>
	TSystem& TSystemStorage<TConfig>::GetSystem()
	{
		static_assert(IsSystem<TSystem>(), "Not a system");

		return std::get<TSystem>(TupleOfSystems);
	}

	template<typename TConfig>
	template<typename TSystem>
	const TSystem& TSystemStorage<TConfig>::GetSystem() const
	{
		static_assert(IsSystem<TSystem>(), "Not a system");

		return std::get<TSystem>(TupleOfSystems);
	}

	template<typename TConfig>
	template<typename TFunc>
	void TSystemStorage<TConfig>::ForSystems(TFunc&& Func)
	{
		ForTuple(TupleOfSystems, std::forward<TFunc>(Func));
	}

	template<typename TConfig>
	template<typename TComponentManager>
	void TSystemStorage<TConfig>::Init(TComponentManager& ComponentManager)
	{
		ForSystems([this, &ComponentManager](auto& System)
		{
			CallInitIfDefined(System, ComponentManager);
		});
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TEnableIf<THasMethod_Init<TSystem, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallInitIfDefined(TSystem& System, TComponentManager& ComponentManager)
	{
		System.Init(ComponentManager);
	}
	
	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TDisableIf<THasMethod_Init<TSystem, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallInitIfDefined(TSystem&, TComponentManager&)
	{
		//No Init defined...don't call
	}

	template<typename TConfig>
	template<typename TComponentManager>
	void TSystemStorage<TConfig>::DeInit(TComponentManager& ComponentManager)
	{
		ForSystems([this, &ComponentManager](auto& System) 
					{
						CallDeInitIfDefined(System, ComponentManager);
					});
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TEnableIf<THasMethod_DeInit<TSystem, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallDeInitIfDefined(TSystem& System, TComponentManager& ComponentManager)
	{
		System.DeInit(ComponentManager);
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TDisableIf<THasMethod_DeInit<TSystem, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallDeInitIfDefined(TSystem&, TComponentManager&)
	{
		//Don't call
	}


	template<typename TConfig>
	template<typename TComponentManager>
	void TSystemStorage<TConfig>::Update(const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
	{
		ForSystems([this, &UpdateEvent, &ComponentManager](auto& System) 
					{
						CallUpdateIfDefined(System, UpdateEvent, ComponentManager);
					});
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TEnableIf<THasMethod_Update<TSystem, FUpdateEvent, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallUpdateIfDefined(TSystem& System, const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
	{
		System.Update(UpdateEvent, ComponentManager);
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TDisableIf<THasMethod_Update<TSystem, FUpdateEvent, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallUpdateIfDefined(TSystem& System, const FUpdateEvent& UpdateEvent, TComponentManager& ComponentManager)
	{
		//Don't call Update if not defined
	}

	template<typename TConfig>
	template<typename TComponentManager>
	void TSystemStorage<TConfig>::OnEvent(const FEvent& Event, TComponentManager& ComponentManager)
	{
		ForSystems([this, &Event, &ComponentManager](auto& System) 
					{
						CallOnEventIfDefined(System, Event, ComponentManager);
					});
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TEnableIf<THasMethod_OnEvent<TSystem, FEvent, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallOnEventIfDefined(TSystem& System, const FEvent& Event, TComponentManager& ComponentManager)
	{
		System.OnEvent(Event, ComponentManager);
	}
	
	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TDisableIf <THasMethod_OnEvent<TSystem, FEvent, TComponentManager>::Value, Int32 >>
		void TSystemStorage<TConfig>::CallOnEventIfDefined(TSystem&, const FEvent&, TComponentManager&)
	{
		//Don't call
	}

	template<typename TConfig>
	template<typename TComponentManager>
	void TSystemStorage<TConfig>::OnEntitiesCreated(const TVector<SizeT>& NewEntityList, TComponentManager& ComponentManager)
	{
		ForSystems([this, &NewEntityList, &ComponentManager](auto& System)
					{
						for (const SizeT NewEntity: NewEntityList)
						{
							CallOnEntityCreatedIfDefined(System, NewEntity, ComponentManager);
						}
					});
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TEnableIf<THasMethod_OnEntityCreated<TSystem, SizeT, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallOnEntityCreatedIfDefined(TSystem& System, SizeT NewEntity, TComponentManager& ComponentManager)
	{
		System.OnEntityCreated(NewEntity, ComponentManager);
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TDisableIf<THasMethod_OnEntityCreated<TSystem, SizeT, TComponentManager>::Value, Int32>>
		void TSystemStorage<TConfig>::CallOnEntityCreatedIfDefined(TSystem&, SizeT, TComponentManager&)
	{}

	template<typename TConfig>
	template<typename TComponentManager>
	void TSystemStorage<TConfig>::OnEntityDestroyed(SizeT DestroyedEntity, TComponentManager& ComponentManager)
	{
		ForSystems([this, DestroyedEntity, &ComponentManager](auto& System) 
					{
						CallOnEntityDestroyedIfDefined(System, DestroyedEntity, ComponentManager);
					});
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TEnableIf<THasMethod_OnEntityDestroyed<TSystem, SizeT, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallOnEntityDestroyedIfDefined(TSystem& System, SizeT EntityID, TComponentManager& ComponentManager)
	{
		System.OnEntityDestroyed(EntityID, ComponentManager);
	}

	template<typename TConfig>
	template<typename TSystem, typename TComponentManager
			, TDisableIf<THasMethod_OnEntityDestroyed<TSystem, SizeT, TComponentManager>::Value, Int32>>
	void TSystemStorage<TConfig>::CallOnEntityDestroyedIfDefined(TSystem&, SizeT, TComponentManager&)
	{}
}

#endif
