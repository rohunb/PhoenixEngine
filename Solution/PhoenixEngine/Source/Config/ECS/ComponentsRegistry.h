#pragma once
#ifndef PHOENIX_COMPONENTS_REGISTRY_H
#define PHOENIX_COMPONENTS_REGISTRY_H

#include "ECS/ComponentManagerConfig.h"
#include "Utility/MetaProgramming/HasInnerType.h"
#include "Utility/MetaProgramming/TypeList.h"
#include "Config/ECS/EngineComponentIncludes.h"
#include "Config/ECS/GameComponentsRegistry.h"

namespace Phoenix
{
	struct FComponentRegistry
	{
		////////////////////
		//EngineComponents
		////////////////////

		using EngineComponentList = TTypeList<CInput, CTransform, CRigidbody, CModel>;

		using EngineTagList = TTypeList<>;

		//Engine Requirements
		using SInputRequirement = TTypeList<CInput>;
		using SPhysicsRequirement = TTypeList<CTransform, CRigidbody>;
		using SRenderRequirement = TTypeList<CTransform, CModel>;

		using EngineRequirementList = TTypeList<SInputRequirement, SPhysicsRequirement, SRenderRequirement>;

		//Engine Systems
		using InputSystem = SInput<SInputRequirement>;
		using PhysicsSystem = SPhysics<SPhysicsRequirement>;
		using RenderSystem = SRender<SRenderRequirement>;

		using EngineSystemList = TTypeList<InputSystem, PhysicsSystem, RenderSystem>;

		/////////
		//Tests
		/////////

		F_DefineTrait_HasInnerType(GameComponentList);
		F_DefineTrait_HasInnerType(GameTagList);
		F_DefineTrait_HasInnerType(GameRequirementList);
		F_DefineTrait_HasInnerType(GameSystemList);

		static_assert(THasInnerType_GameComponentList<FGameComponentRegistry>::Value
					 , "FGameComponentRegistry should define a List of Components as GameComponentList");

		static_assert(THasInnerType_GameTagList<FGameComponentRegistry>::Value
					 , "FGameComponentRegistry should define a List of Tags as GameTagList");

		static_assert(THasInnerType_GameTagList<FGameComponentRegistry>::Value
					 , "FGameComponentRegistry should define a List of Tags as GameTagList");

		static_assert(THasInnerType_GameRequirementList<FGameComponentRegistry>::Value
					 , "FGameComponentRegistry should define a List of Requirements as GameRequirementList");

		static_assert(THasInnerType_GameSystemList<FGameComponentRegistry>::Value
					 , "FGameComponentRegistry should define a List of Systems as GameSystemList");

		/////////////////////////
		//Aggregated Components
		/////////////////////////

		using ComponentList = TConcat<EngineComponentList, FGameComponentRegistry::GameComponentList>;

		using TagList = TConcat<EngineTagList, FGameComponentRegistry::GameTagList>;

		using RequirementList = TConcat<EngineRequirementList, FGameComponentRegistry::GameRequirementList>;

		using SystemList = TConcat<EngineSystemList, FGameComponentRegistry::GameSystemList>;

		using Config = TComponentManagerConfig<ComponentList, TagList, RequirementList, SystemList>;
	};
}

#endif

