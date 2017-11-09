#include "Stdafx.h"
// #FIXME: This needs to be moved to FPhysicsEngine
#include "Components/Cores/PhysicsCore.h"

#include "Platform/Event/Event.h"
#include "Components/Transform.h"
#include "Components/ComponentFilter.h"

using namespace Phoenix;

FPhysicsCore::FPhysicsCore() : Base(ComponentFilter().Requires<Transform>())
{
}

FPhysicsCore::~FPhysicsCore()
{
}

void FPhysicsCore::Init()
{
	Config = new btDefaultCollisionConfiguration();
	Dispatcher = new btCollisionDispatcher(Config);
	BP = new btDbvtBroadphase();
	Solver = new btSequentialImpulseConstraintSolver;
	World = new btDiscreteDynamicsWorld(Dispatcher, BP, Solver, Config);

	World->setGravity(btVector3(0, -10, 0));

	BulletTestCode();
}

void FPhysicsCore::Update(const FUpdateEvent& UpdateEvent)
{
	World->stepSimulation(UpdateEvent.DeltaTimeS);

	auto PhysicsObjects = GetEntities();
	for (auto& InEntity : PhysicsObjects)
	{

	}
}

void FPhysicsCore::DeInit()
{
	delete World;
	World = nullptr;

	delete Solver;
	Solver = nullptr;

	delete BP;
	BP = nullptr;

	delete Dispatcher;
	Dispatcher = nullptr;

	delete Config;
	Config = nullptr;
}

void Phoenix::FPhysicsCore::BulletTestCode()
{
	int i;

	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -56, 0));

	{
		btScalar mass(0.);

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		World->addRigidBody(body);
	}


	{
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(1.f);

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		World->addRigidBody(body);
	}
	for (i = 0; i < 5; i++)
	{
		World->stepSimulation(1.f / 60.f, 10);

		for (int j = World->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = World->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);

			}
			else
			{
				trans = obj->getWorldTransform();
			}
			//F_Log("Bullet test object location: " << j << " = " << float(trans.getOrigin().getX()) << ", " << float(trans.getOrigin().getY()) << ", " << float(trans.getOrigin().getZ()));
		}
	}

	for (i = World->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = World->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		World->removeCollisionObject(obj);
		delete obj;
	}

	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	collisionShapes.clear();
}
