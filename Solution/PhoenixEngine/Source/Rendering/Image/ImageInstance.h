#ifndef PHOENIX_IMAGE_INSTANCE_H
#define PHOENIX_IMAGE_INSTANCE_H

#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/String.h"
#include "Math/Vector2D.h"
#include "Math/Vector4D.h"

namespace Phoenix
{
	class FImageInstance
	{
	public:
		void SetColor(const FVector4D& Color);
		void SetColor(const Float32 R, const Float32 G, const Float32 B, const Float32 A);

		void SetFlip(const FVector2D& Flip);
		void SetFlip(const Float32 XFlip, const Float32 YFlip);

		void SetImage(const FString& Image);
		void SetImage(FString&& Image);

		void SetLayer(const UInt32 Layer);

		void SetPosition(const FVector2D& Position);
		void SetPosition(const Float32 X, const Float32 Y);

		void SetScale(const FVector2D& Scale);
		void SetScale(const Float32 X, const Float32 Y);

		void SetViewRect(const FVector4D& ViewRect);
		void SetViewRect(const Float32 X, const Float32 Y, const Float32 Width, const Float32 Height);

		const FVector4D& GetColor() const;
		const FVector2D& GetFlip() const;
		const FString& GetImage() const;
		UInt32 GetLayer() const;
		const FVector2D& GetPosition() const;
		const FVector2D& GetScale() const;
		const FVector4D& GetViewRectangle() const;

	private:
		FVector4D Color{ 1.f, 1.f, 1.f, 1.f };
		FVector2D Position{ 0.f, 0.f };
		FVector2D Scale{ 1.f, 1.f };
		FVector2D Flip{ 1.f, 1.f };
		FVector4D ViewRect{ 0.f, 0.f, 1.f, 1.f };
		UInt32 Layer{ 0 };
		FString Image;
	};
}

#endif
