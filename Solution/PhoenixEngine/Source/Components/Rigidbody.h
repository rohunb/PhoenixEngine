#pragma once
#include "Components/Component.h"
#include "Math/Vector3D.h"

#include "Bullet/btBulletDynamicsCommon.h"

namespace Phoenix
{
	class FRigidbody :
		public Component<FRigidbody>
	{
	public:
		F_GeneratedComponentClass(FRigidbody);
		virtual ~FRigidbody();

		virtual void Init() final;

	private:
		btRigidBody* Rigidbody{ nullptr };
	};
}