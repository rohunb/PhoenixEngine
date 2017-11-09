#ifndef PHOENIX_GBUFFER_H
#define PHOENIX_GBUFFER_H

#include "Utility/Containers/Array.h"
#include "Utility/Misc/Primitives.h"
#include "Rendering/GL/GLInterface.h"

namespace Phoenix
{
	namespace EGBufferTex
	{
		typedef GLsizei Type;

		enum Value : Type
		{
			Color,
			Count
		};
	}

	class FGBuffer
	{
	public:
		FGBuffer();

		FGBuffer(const FGBuffer&) = delete;
		FGBuffer& operator=(const FGBuffer&) = delete;

		FGBuffer(FGBuffer&&) = delete;
		FGBuffer& operator=(FGBuffer&&) = delete;
		
		~FGBuffer();

		void Init(const GLsizei Width, const GLsizei Height);

		bool IsValid() const;

		void DeInit();

		void EnableWrite();

		void DisableWrite();

		void EnableRead();

		void DisableRead();

		void BindTexture(const EGBufferTex::Value Texture);

	protected:
	private:
		static const TArray<GL::EAttachment::Value, EGBufferTex::Count> ColorAttachments;

		GL::FrameBufferT Framebuffer{ 0 };
		TArray<GL::ImageIDT, EGBufferTex::Count> Textures;
		GL::ImageIDT DepthStencilTex{ 0 };
	};
}

#endif
