#ifndef PHOENIX_MODEL_INSTANCE_H
#define PHOENIX_MODEL_INSTANCE_H

#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/String.h"
#include "Math/Quaternion.h"
#include "Math/Vector3D.h"
#include "Rendering/Material.h"

namespace Phoenix
{
	class FModelInstance
	{
	public:
		void SetMaterial(const FMaterial& Material);
		void SetModel(const FString& Model);
		void SetModel(FString&& Model);
		void SetOrigin(const FVector3D& Origin);
		void SetPosition(const FVector3D& Position);
		void SetRotation(const FQuaternion& Rotation);
		void SetScale(const FVector3D& Scale);

		const FMaterial& GetMaterial() const;
		const FString& GetModel() const;
		const FVector3D& GetOrigin() const;
		const FVector3D& GetPosition() const;
		const FQuaternion& GetRotation() const;
		const FVector3D& GetScale() const;

	private:
		FVector3D Origin;
		FVector3D Position;
		FVector3D Scale{ 1.f };
		FQuaternion Rotation;
		FString Model;
		FMaterial Material;
	};
}

#endif
