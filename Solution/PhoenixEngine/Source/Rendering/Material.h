#pragma once

#include "Utility/Containers/Array.h"
#include "Utility/Misc/Primitives.h"
#include "Math/Vector2D.h"
#include "Math/Vector4D.h"

namespace Phoenix
{
	namespace EMaterialType
	{
		typedef UInt8 Type;

		enum Value : Type
		{
			Blinn,
			Cook,
			Strauss,
			Ward,
			Ashik,
			Count
		};

		static bool IsValid(const EMaterialType::Type MaterialType);
	}

	struct FMaterialBase
	{
		static FVector2D ColorRange;

		EMaterialType::Value MaterialType;
		FVector4D Color;

		static FMaterialBase Create(
			const EMaterialType::Value MaterialType,
			const FVector4D& Color);
	};

	struct FAshikMaterial
	{
		static const FVector2D AnisotropicRange;

		Float32 AnisotropicU;
		Float32 AnisotropicV;

		static FAshikMaterial Create(
			const Float32 AnisotropicU,
			const Float32 AnisotropicV);
	};

	struct FBlinnMaterial
	{
		static const FVector2D SpecExpRange;

		Float32 SpecularExp;

		static FBlinnMaterial Create(
			const Float32 SpecularExp);
	};

	struct FCookMaterial
	{
		static const FVector2D RoughnessRange;
		static const FVector2D RefANIRange;

		Float32 Roughness;
		Float32	RefANI;

		static FCookMaterial Create(
			const Float32 Roughness,
			const Float32 RefANI);
	};

	struct FStraussMaterial
	{
		static const FVector2D SmoothnessRange;
		static const FVector2D MetalnessRange;
		static const FVector2D TransparencyRange;

		Float32 Smoothness;
		Float32 Metalness;
		Float32 Transparency;

		static FStraussMaterial Create(
			const Float32 Smoothness,
			const Float32 Metalness,
			const Float32 Transparency);
	};

	struct FWardMaterial
	{
		static const FVector2D RoughnessRange;

		Float32 Roughness;

		static FWardMaterial Create(
			const Float32 Roughness);
	};

	struct FMaterial
	{
		FMaterialBase Base;
		union
		{
			FAshikMaterial Ashik;
			FBlinnMaterial Blinn;
			FCookMaterial Cook;
			FStraussMaterial Strauss;
			FWardMaterial Ward;
		};

		static FMaterial CreateDefault(
			const FVector4D& Color = FVector4D(1.f));

		static FMaterial CreateAshik(
			const FVector4D& Color,
			const Float32 AnisotropicU,
			const Float32 AnisotropicV);

		static FMaterial CreateBlinn(
			const FVector4D& Color,
			const Float32 SpecularExp);

		static FMaterial CreateCook(
			const FVector4D& Color,
			const Float32 Roughness,
			const Float32 RefANI);

		static FMaterial CreateStrauss(
			const FVector4D& Color,
			const Float32 Smoothness,
			const Float32 Metalness,
			const Float32 Transparency);

		static FMaterial CreateWard(
			const FVector4D& Color,
			const Float32 Roughness);
	};
}
