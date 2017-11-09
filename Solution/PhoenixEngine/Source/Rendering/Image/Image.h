#ifndef PHOENIX_IMAGE_H
#define PHOENIX_IMAGE_H

#include "ExternalLib/GLIncludes.h"
#include "Utility/Misc/Primitives.h"
#include "Math/Vector2D.h"
#include "Rendering/GL/GLInterface.h"
#include "Rendering/Image/ImageData.h"

namespace Phoenix
{
	class FImage
	{
	public:
		struct FInitParams
		{
			const FImageData* ImageData{ nullptr };
			GL::MipmapLevelT MipmapLevel{ 0 };

			GL::ETexWrap::Value TexWrapS{ GL::ETexWrap::ClampToEdge };
			GL::ETexWrap::Value TexWrapT{ GL::ETexWrap::ClampToEdge };

			GL::ETexMinFilter::Value TexMinFilter{ GL::ETexMinFilter::Linear };
			GL::ETexMagFilter::Value TexMagFilter{ GL::ETexMagFilter::Linear };
		};

		FImage() = default;

		FImage(const FImage&) = delete;
		FImage& operator=(const FImage&) = delete;

		FImage(FImage&&);
		FImage& operator=(FImage&&);

		void Init(const FInitParams& InitParams);

		bool IsValid() const;

		void DeInit();

		void Enable() const;

		void Disable();

		GL::ImageIDT GetID() const;

		FVector2D GetDimensions() const;

		EPixelFormat::Value GetPixelFormat();

	protected:
	private:
		GL::ImageIDT ID{ 0 };
		FImageData::WidthT Width{ 0 };
		FImageData::HeightT Height{ 0 };
		EPixelFormat::Value PixelFormat{ EPixelFormat::Unknown };

		void PostMoveReset();
	};
}

#endif
