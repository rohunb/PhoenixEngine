#pragma once
// #FIXME: This needs to be moved to FPhysicsEngine
#include "Components/Core.h"

// Bullet includes
#include "Bullet/btBulletDynamicsCommon.h"

namespace Phoenix
{
	class FPhysicsCore :
		public FCore<FPhysicsCore>
	{
	public:
		FPhysicsCore();
		~FPhysicsCore();

		virtual void Init() final;

		virtual void DeInit() final;

		virtual void Update(const struct FUpdateEvent& UpdateEvent) final;

	private:
		btBroadphaseInterface* BP{ nullptr };
		btDefaultCollisionConfiguration* Config{ nullptr };
		btCollisionDispatcher* Dispatcher{ nullptr };
		btSequentialImpulseConstraintSolver* Solver{ nullptr };

		btDiscreteDynamicsWorld* World{ nullptr };

		void BulletTestCode();
	};
}