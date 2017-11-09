#ifndef PHOENIX_IMAGE_PROCESSOR_H
#define PHOENIX_IMAGE_PROCESSOR_H

#include "Utility/Misc/Primitives.h"
#include "Rendering/Image/ImageData.h"

namespace Phoenix
{
	class FImageProcessor
	{
		friend struct FImageProcessorHelper;
	public:
		struct FLoadParams
		{
			const FChar* File{ nullptr };
			EPixelFormat::Value ImageLayout{ EPixelFormat::Unknown };

			FLoadParams() = default;
			FLoadParams(const char* File, const EPixelFormat::Value ImageLayout);
		};

		FImageProcessor() = default;
		~FImageProcessor();

		FImageProcessor(const FImageProcessor&) = delete;
		FImageProcessor& operator=(const FImageProcessor&) = delete;

		FImageProcessor(FImageProcessor&&);
		FImageProcessor& operator=(FImageProcessor&&);

		void Load(const FLoadParams& LoadParams);

		bool IsValid() const;

		void Unload();

		const FImageData& GetImageData() const;

	private:
		static const FImageData::WidthT MaxWidth;
		static const FImageData::HeightT MaxHeight;

		FImageData Data;

		void PostMoveReset();
	};
}

#endif
