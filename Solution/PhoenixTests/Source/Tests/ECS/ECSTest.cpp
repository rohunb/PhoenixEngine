#include "Tests/ECS/ECSTest.h"

#include "ECS/ComponentManager.h"
#include "ECS/ComponentManagerConfig.h"
#include "ECS/RequirementBitArrayStorage.h"
#include "Math/Vector3D.h"
#include "Utility/Debug/Assert.h"
#include "Utility/MetaProgramming/TypeList.h"

#include <iostream>

using namespace Phoenix;

namespace ECSTestStructs
{
	struct Comp0 {};
	struct Comp1 {};
	struct Comp2 {};

	using ComponentList = TTypeList<Comp0, Comp1, Comp2>;

	struct Tag0 {};
	struct Tag1 {};

	using TagList = TTypeList<Tag0, Tag1>;

	using Requirement0 = TTypeList<>;
	using Requirement1 = TTypeList<Tag0, Comp0, Comp2>;
	using Requirement2 = TTypeList<Tag1, Comp2, Comp1>;
	using Requirement3 = TTypeList<Comp0, Comp1>;

	using RequirementList = TTypeList<Requirement0, Requirement1, Requirement2, Requirement3>;

	template<typename TRequirement>
	struct TSystem0 {};

	using System0 = TSystem0<Requirement1>;

	using SystemList = TTypeList<System0>;

	using Config = TComponentManagerConfig<ComponentList, TagList, RequirementList, SystemList>;
}

void FECSTest::RunTests() const
{
	TestRequirementBitArrays();
	TestComponentManager();
}

void FECSTest::TestRequirementBitArrays() const
{
	using namespace ECSTestStructs;

	TRequirementBitArrayStorage<Config> RequirementBitArrays;

	const auto& RequirementBitArray0 = RequirementBitArrays.GetRequirementBitArray<Requirement0>();
	const auto& RequirementBitArray1 = RequirementBitArrays.GetRequirementBitArray<Requirement1>();
	const auto& RequirementBitArray2 = RequirementBitArrays.GetRequirementBitArray<Requirement2>();
	const auto& RequirementBitArray3 = RequirementBitArrays.GetRequirementBitArray<Requirement3>();

	//BitArray Arrangement: {Tag1, Tag0, Comp2, Comp1, Comp0}

	using BitArray = Config::ComponentsBitArray;
	F_AssertEqual(RequirementBitArray0, BitArray { "00000" }, "Requirement Bit Array is Incorrect");
	F_AssertEqual(RequirementBitArray1, BitArray { "01101" }, "Requirement Bit Array is Incorrect");
	F_AssertEqual(RequirementBitArray2, BitArray { "10110" }, "Requirement Bit Array is Incorrect");
	F_AssertEqual(RequirementBitArray3, BitArray { "00011" }, "Requirement Bit Array is Incorrect");
}

void FECSTest::TestComponentManager() const
{
	ManagerBasicTests();
	ManagerKillTests();
	ManagerComponentTests();
}

void FECSTest::ManagerBasicTests() const
{
	using namespace ECSTestStructs;

	TComponentManager<Config> ComponentManager;
	std::cout << "Component Manager Initialized...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 0, "Component Manager incorrect state");

	using EntityID = TComponentManager<Config>::EntityID;

	EntityID Entity0 = ComponentManager.CreateEntity();
	std::cout << "First Entity Created...\n";
	ComponentManager.PrintState();

	EntityID Entity1 = ComponentManager.CreateEntity();
	std::cout << "Second Entity Created...\n";
	ComponentManager.PrintState();

	F_AssertFalse(ComponentManager.HasTag<Tag0>(Entity0), "Tag error");
	ComponentManager.AddTag<Tag0>(Entity0);
	F_Assert(ComponentManager.HasTag<Tag0>(Entity0), "Tag error");

	F_AssertFalse(ComponentManager.HasTag<Tag1>(Entity1), "Tag error");
	ComponentManager.AddTag<Tag1>(Entity1);
	F_Assert(ComponentManager.HasTag<Tag1>(Entity1), "Tag error");

	F_Assert(ComponentManager.HasTag<Tag0>(Entity0), "Tag error");
	ComponentManager.RemoveTag<Tag0>(Entity0);
	F_AssertFalse(ComponentManager.HasTag<Tag0>(Entity0), "Tag error");

	F_AssertFalse(ComponentManager.HasComponent<Comp0>(Entity0), "Component error");
	ComponentManager.AddComponent<Comp0>(Entity0);
	F_Assert(ComponentManager.HasComponent<Comp0>(Entity0), "Component error");

	F_AssertFalse(ComponentManager.HasComponent<Comp1>(Entity1), "Component error");
	ComponentManager.AddComponent<Comp1>(Entity1);
	F_Assert(ComponentManager.HasComponent<Comp1>(Entity1), "Component error");

	F_Assert(ComponentManager.HasComponent<Comp0>(Entity0), "Component error");
	ComponentManager.RemoveComponent<Comp0>(Entity0);
	F_AssertFalse(ComponentManager.HasComponent<Comp0>(Entity0), "Component error");

	F_AssertEqual(ComponentManager.GetEntityCount(), 0, "Entity count should remain at 0 until Refreshed");

	ComponentManager.Refresh();

	std::cout << "After Refresh...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 2, "Entity count should be updated after Refresh");

	F_AssertFalse(ComponentManager.HasTag<Tag0>(Entity0), "Tag error");
	F_Assert(ComponentManager.HasTag<Tag1>(Entity1), "Tag error");

	F_AssertFalse(ComponentManager.HasComponent<Comp0>(Entity0), "Component error");
	F_Assert(ComponentManager.HasComponent<Comp1>(Entity1), "Component error");

	ComponentManager.Clear();

	std::cout << "After Clear...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 0, "Entity count be 0 after Clear");
}

void FECSTest::ManagerKillTests() const
{
	using namespace ECSTestStructs;

	TComponentManager<Config> ComponentManager;
	using EntityID = TComponentManager<Config>::EntityID;

	//Fill up...assuming 10 initial size
	const SizeT StartingSize = 10;
	F_AssertEqual(TComponentManager<Config>::StartingSize, StartingSize, "Update starting size");

	for (SizeT I = 0; I < StartingSize; ++I)
	{
		ComponentManager.CreateEntity();
	}

	F_AssertEqual(ComponentManager.GetEntityCount(), 0, "Entity Count incorrect");

	std::cout << "Filled starting entities...\n";
	ComponentManager.PrintState();
	ComponentManager.Refresh();
	std::cout << "Refreshed...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 10, "Entity Count incorrect");

	EntityID Entity10 = ComponentManager.CreateEntity();
	F_AssertEqual(ComponentManager.GetEntityCount(), 10, "Entity Count incorrect");

	std::cout << "11th entity...\n";
	ComponentManager.PrintState();
	ComponentManager.Refresh();
	std::cout << "Refreshed...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 11, "Entity Count incorrect");
	F_AssertEqual(ComponentManager.GetCapacity(), 20, "Capacity incorrect");

	ComponentManager.Destroy(Entity10);

	F_AssertEqual(ComponentManager.GetEntityCount(), 11, "Entity Count incorrect");

	std::cout << "11th entity killed...\n";
	ComponentManager.PrintState();
	ComponentManager.Refresh();
	std::cout << "Refreshed...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 10, "Entity Count incorrect");
	F_AssertEqual(ComponentManager.GetCapacity(), 20, "Capacity incorrect");

	Entity10 = ComponentManager.CreateEntity();
	EntityID Entity11 = ComponentManager.CreateEntity();

	F_AssertEqual(ComponentManager.GetEntityCount(), 10, "Entity Count incorrect");

	std::cout << "12th entity...\n";
	ComponentManager.PrintState();
	ComponentManager.Refresh();
	std::cout << "Refreshed...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 12, "Entity Count incorrect");

	ComponentManager.Destroy(0);

	F_AssertEqual(ComponentManager.GetEntityCount(), 12, "Entity Count incorrect");

	std::cout << "0th entity killed...\n";
	ComponentManager.PrintState();
	ComponentManager.Refresh();
	std::cout << "Refreshed...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 11, "Entity Count incorrect");

	ComponentManager.Destroy(5);

	F_AssertEqual(ComponentManager.GetEntityCount(), 11, "Entity Count incorrect");

	std::cout << "5th entity killed...\n";
	ComponentManager.PrintState();
	ComponentManager.Refresh();
	std::cout << "Refreshed...\n";
	ComponentManager.PrintState();

	F_AssertEqual(ComponentManager.GetEntityCount(), 10, "Entity Count incorrect");

	ComponentManager.ForEntities([&ComponentManager](EntityID ID) 
								{
									const auto& Entity = ComponentManager.GetEntityByID(ID);
									const auto& BitArray = Entity.BitArray;
									std::cout << BitArray << std::endl;
								});
}

void FECSTest::ManagerComponentTests() const
{
	struct CTransform
	{
		FVector3D Position;

		CTransform() = default;

		CTransform(const FVector3D& Position)
			: Position(Position)
		{}
	};

	struct CRigidBody
	{
		FVector3D Velocity;

		CRigidBody() = default;

		CRigidBody(const FVector3D& Velocity)
			: Velocity(Velocity)
		{}
	};

	using ComponentList = TTypeList<CTransform, CRigidBody>;
	using TagList = TTypeList<>;
	using RequirementList = TTypeList<>;
	using SystemList = TTypeList<>;
	using Config = TComponentManagerConfig<ComponentList, TagList, RequirementList, SystemList>;

	TComponentManager<Config> ComponentManager;
	typedef TComponentManager<Config>::EntityID EntityID;

	EntityID Entity0 = ComponentManager.CreateEntity();
	ComponentManager.Refresh();

	FVector3D Position { 1.0f, 1.0f, 1.0f };
	ComponentManager.AddComponent<CTransform>(Entity0, Position);
	F_Assert(ComponentManager.HasComponent<CTransform>(Entity0), "Add Component failed");

	CTransform& Transform = ComponentManager.GetComponent<CTransform>(Entity0);
	F_AssertEqual(Transform.Position, Position, "Component data incorrect");

	FVector3D DefaultVector;
	FVector3D HighVelocity { 10.0f, 20.0f, 30.0f };

	ComponentManager.AddComponent<CRigidBody>(Entity0);
	F_Assert(ComponentManager.HasComponent<CRigidBody>(Entity0), "Add Component failed");

	CRigidBody& Rigidbody = ComponentManager.GetComponent<CRigidBody>(Entity0);
	F_AssertEqual(Rigidbody.Velocity, DefaultVector, "Component data incorrect");

	ComponentManager.RemoveComponent<CRigidBody>(Entity0);
	F_AssertFalse(ComponentManager.HasComponent<CRigidBody>(Entity0), "Remove Component failed");

	ComponentManager.AddComponent<CRigidBody>(Entity0, HighVelocity);
	F_Assert(ComponentManager.HasComponent<CRigidBody>(Entity0), "Add Component failed");

	CRigidBody& NewRigidbody = ComponentManager.GetComponent<CRigidBody>(Entity0);
	F_AssertEqual(NewRigidbody.Velocity, HighVelocity, "Component data incorrect");

	EntityID Entity1 = ComponentManager.CreateEntity();
	ComponentManager.AddComponent<CTransform>(Entity1, FVector3D(2.0f, 2.0f, 2.0f));
	ComponentManager.AddComponent<CRigidBody>(Entity1, FVector3D(2.0f, 2.0f, 2.0f));

	EntityID Entity2 = ComponentManager.CreateEntity();
	ComponentManager.AddComponent<CTransform>(Entity2, FVector3D(4.0f, 4.0f, 4.0f));
	ComponentManager.AddComponent<CRigidBody>(Entity2, FVector3D(4.0f, 4.0f, 4.0f));

	ComponentManager.Refresh();

	CTransform& Transform1 = ComponentManager.GetComponent<CTransform>(Entity1);
	F_AssertEqual(Transform1.Position, FVector3D(2.0f,2.0f,2.0f), "Component data incorrect");
	CRigidBody& Rigidbody1 = ComponentManager.GetComponent<CRigidBody>(Entity1);
	F_AssertEqual(Rigidbody1.Velocity, FVector3D(2.0f, 2.0f, 2.0f), "Component data incorrect");

	ComponentManager.Destroy(Entity1);
	ComponentManager.Refresh();

	EntityID Entity3 = ComponentManager.CreateEntity();
	ComponentManager.AddComponent<CTransform>(Entity3, FVector3D(3.0f, 3.0f, 3.0f));
	ComponentManager.AddComponent<CRigidBody>(Entity3, FVector3D(3.0f, 3.0f, 3.0f));

	ComponentManager.Refresh();

	CTransform& Transform2 = ComponentManager.GetComponent<CTransform>(Entity3);
	F_AssertEqual(Transform2.Position, FVector3D(3.0f, 3.0f, 3.0f), "Component data incorrect");
	CRigidBody& Rigidbody2 = ComponentManager.GetComponent<CRigidBody>(Entity3);
	F_AssertEqual(Rigidbody2.Velocity, FVector3D(3.0f, 3.0f, 3.0f), "Component data incorrect");
}
