#ifndef GAME_START_SCENE_H
#define GAME_START_SCENE_H

#include "Core/GameScene.h"

namespace Phoenix
{
	class FStartScene : public FGameScene
	{
	public:
	protected:

		virtual void OnInit(FComponentManager& ComponentManager) override;

		virtual void OnUpdate(const struct FUpdateEvent& UpdateEvent, FComponentManager& ComponentManager) override;

		virtual void OnDeInit() override;
	};
}

#endif