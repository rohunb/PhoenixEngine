#include "StartScene.h"

#include "Platform/Event/Event.h"
#include "Utility/Containers/Vector.h"

using namespace Phoenix;

void FStartScene::OnInit(FComponentManager& ComponentManager)
{
	/*auto& myObj = this->CreateGameObject();
	myObj.AddComponent<Transform>();*/

	typedef SizeT EntityID;

	const FChar* const GolemFileName = "golem.pmesh";

	/*{
		EntityID SmallEntity = ComponentManager.CreateEntity();
		ComponentManager.AddComponent<CModel>(SmallEntity, GolemFileName);
		ComponentManager.AddComponent<CTransform>(SmallEntity, FVector3D(150.0f, 100.0f, 0.0f));
		ComponentManager.AddComponent<CRigidbody>(SmallEntity, FVector3D(), FVector3D(0.0f, -20.0f, 0.0f));
	}
	{
		EntityID BigEntity = ComponentManager.CreateEntity();
		ComponentManager.AddComponent<CModel>(BigEntity, GolemFileName);
		ComponentManager.AddComponent<CTransform>(BigEntity, FVector3D(-150.0f, 100.0f, 0.0f), FVector3D(1.3f), FQuaternion());
		ComponentManager.AddComponent<CRigidbody>(BigEntity, FVector3D(), FVector3D(0.0f, -100.0f, 0.0f));
	}
	{
		EntityID SidewaysEntity = ComponentManager.CreateEntity();
		ComponentManager.AddComponent<CModel>(SidewaysEntity, GolemFileName);
		ComponentManager.AddComponent<CTransform>(SidewaysEntity, FVector3D(0.0f, 0.0f, 0.0f));
		ComponentManager.AddComponent<CRigidbody>(SidewaysEntity, FVector3D(50.0f, 0.0f, 0.0f));
		ComponentManager.AddComponent<CMoveSideways>(SidewaysEntity, 150.0f);
	}
	{
		EntityID FastSidewaysEntity = ComponentManager.CreateEntity();
		ComponentManager.AddComponent<CModel>(FastSidewaysEntity, GolemFileName);
		ComponentManager.AddComponent<CTransform>(FastSidewaysEntity, FVector3D(0.0f, 0.0f, 0.0f));
		ComponentManager.AddComponent<CRigidbody>(FastSidewaysEntity, FVector3D(-150.0f, 0.0f, 0.0f));
		ComponentManager.AddComponent<CMoveSideways>(FastSidewaysEntity, 150.0f);
	}*/
	{
		EntityID PlayerEntity = ComponentManager.CreateEntity();
		ComponentManager.AddComponent<CModel>(PlayerEntity, GolemFileName);

		ComponentManager.AddComponent<CTransform>(PlayerEntity);
		ComponentManager.AddComponent<CRigidbody>(PlayerEntity);

		ComponentManager.AddComponent<CMoveSpeed>(PlayerEntity, 3.0f);
		ComponentManager.AddComponent<CInput>(PlayerEntity);

		ComponentManager.AddComponent<CGolemCannon>(PlayerEntity);

		ComponentManager.AddTag<TPlayer>(PlayerEntity);
	}
	{
		EntityID GolemSpawner = ComponentManager.CreateEntity();
		ComponentManager.AddComponent<CGolemSpawnTime>(GolemSpawner);
	}

	ComponentManager.Refresh();
}

void FStartScene::OnUpdate(const FUpdateEvent& UpdateEvent, FComponentManager& ComponentManager)
{
}

void FStartScene::OnDeInit()
{

}