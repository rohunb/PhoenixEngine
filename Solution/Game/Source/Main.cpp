#include "Core/Engine.h"
#include "Scenes/StartScene.h"

using namespace Phoenix;

static TUniquePtr<FGameScene> CreateGameSceneFunc()
{
	return std::make_unique<FStartScene>();
}

int main()
{
	{
		FEngine Engine;
		Engine.Run(CreateGameSceneFunc);
	}
	return 0;
}
