#ifndef PHOENIX_GAME_COMPONENTS_REGISTRY_H
#define PHOENIX_GAME_COMPONENTS_REGISTRY_H

#include "Config/ECS/GameComponentIncludes.h"
#include "Config/ECS/EngineComponentIncludes.h"
#include "Utility/MetaProgramming/TypeList.h"

namespace Phoenix
{
	struct FGameComponentRegistry
	{
		//ComponentList
		using GameComponentList = TTypeList	< CGolemCannon
											, CGolemSpawnTime
											, CMoveSideways
											, CMoveSpeed
											, CRandomSpin
											, CTimedDestruction
											>;

		//TagList
		using GameTagList = TTypeList<TPlayer, TEnemy>;

		//Requirements
		using SGolemCannonRequirement = TTypeList<CGolemCannon, CTransform, CInput>;
		using SMoveSidewaysRequirement = TTypeList<CTransform, CRigidbody, CMoveSideways>;
		using SMoveOnInputRequirement = TTypeList<CMoveSpeed, CRigidbody, CInput>;
		using SRandomSpinRequirement = TTypeList<CTransform, CRandomSpin>;
		using STimedDestructionRequirement = TTypeList<CTimedDestruction>;
		using STimedSpawnGolemRequirement = TTypeList<CGolemSpawnTime>;

		//RequirementList
		using GameRequirementList = TTypeList < SGolemCannonRequirement
											  , SMoveSidewaysRequirement
											  , SMoveOnInputRequirement
											  , SRandomSpinRequirement
											  , STimedDestructionRequirement
											  , STimedSpawnGolemRequirement
											  >;

		//Systems
		using GolemCannonSystem = SGolemCannon<SGolemCannonRequirement>;
		using MoveOnInputSystem = SMoveOnInput<SMoveOnInputRequirement>;
		using MoveSidewaysSystem = SMoveSideways<SMoveSidewaysRequirement>;
		using RandomSpinSystem = SRandomSpin<SRandomSpinRequirement>;
		using TimedDestructionSystem = STimedDestruction<STimedDestructionRequirement>;
		using TimedGolemSpawnSystem = STimedSpawnGolem<STimedSpawnGolemRequirement>;

		//SystemList
		using GameSystemList = TTypeList < GolemCannonSystem
										 , MoveOnInputSystem
										 , MoveSidewaysSystem
										 , RandomSpinSystem
										 , TimedDestructionSystem
										 , TimedGolemSpawnSystem
										 >;
	};
}

#endif
