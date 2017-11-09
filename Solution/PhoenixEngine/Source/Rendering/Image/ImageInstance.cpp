#include "Stdafx.h"
#include "Rendering/Image/ImageInstance.h"

using namespace Phoenix;

void FImageInstance::SetColor(const FVector4D& InColor)
{
	Color = InColor;
}

void FImageInstance::SetColor(const Float32 R, const Float32 G, const Float32 B, const Float32 A)
{
	Color = FVector4D(R, G, B, A);
}

void FImageInstance::SetFlip(const FVector2D& InFlip)
{
	Flip = InFlip;
}

void FImageInstance::SetFlip(const Float32 XFlip, const Float32 YFlip)
{
	Flip = FVector2D(XFlip, YFlip);
}

void FImageInstance::SetImage(const FString& InImage)
{
	Image = InImage;
}

void FImageInstance::SetImage(FString&& InImage)
{
	Image = std::move(InImage);
}

void FImageInstance::SetLayer(const UInt32 InLayer)
{
	Layer = InLayer;
}

void FImageInstance::SetPosition(const FVector2D& InPosition)
{
	Position = InPosition;
}

void FImageInstance::SetPosition(const Float32 X, const Float32 Y)
{
	Position = FVector2D(X, Y);
}

void FImageInstance::SetScale(const FVector2D& InScale)
{
	Scale = InScale;
}

void FImageInstance::SetScale(const Float32 X, const Float32 Y)
{
	Scale = FVector2D(X, Y);
}

void FImageInstance::SetViewRect(const FVector4D& InViewRect)
{
	ViewRect = InViewRect;
}

void FImageInstance::SetViewRect(const Float32 X, const Float32 Y, const Float32 Width, const Float32 Height)
{
	ViewRect = FVector4D(X, Y, Width, Height);
}

const FVector4D& FImageInstance::GetColor() const
{
	return Color;
}

const FVector2D& FImageInstance::GetFlip() const
{
	return Flip;
}

const FString& FImageInstance::GetImage() const
{
	return Image;
}

UInt32 FImageInstance::GetLayer() const
{
	return Layer;
}

const FVector2D& FImageInstance::GetPosition() const
{
	return Position;
}

const FVector2D& FImageInstance::GetScale() const
{
	return Scale;
}

const FVector4D& FImageInstance::GetViewRectangle() const
{
	return ViewRect;
}