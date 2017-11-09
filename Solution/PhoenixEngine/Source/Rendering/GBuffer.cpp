#include "Stdafx.h"
#include "Rendering/GBuffer.h"

#include "Utility/Debug/Assert.h"
#include "Rendering/Debug/GFXDebug.h"

using namespace Phoenix;
using namespace Phoenix::GL;

static_assert(EGBufferTex::Count == 1, "This array requires an update.");
const TArray<EAttachment::Value, EGBufferTex::Count> FGBuffer::ColorAttachments =
{
	EAttachment::Color0
};

FGBuffer::FGBuffer()
{
	Textures.fill(0);
}

FGBuffer::~FGBuffer()
{
	DeInit();
}

void FGBuffer::Init(const GLsizei Width, const GLsizei Height)
{
	F_Assert(Width > 0, "Width must be greater than zero, but a value of " << Width << " was provided.");
	F_Assert(Height > 0, "Height must be greater than zero, but a value of " << Height << " was provided.");

	F_GL(GL::GenFramebuffers(1, &Framebuffer));
	F_GL(GL::BindFramebuffer(EGBuffer::Framebuffer, Framebuffer));

	F_GL(GL::GenTextures(EGBufferTex::Count, &Textures[0]));
	F_GL(GL::GenTextures(1, &DepthStencilTex));

	static_assert(EGBufferTex::Count == 1, "These arrays require an update.");
	const TArray<ETexGLFormat::Value, EGBufferTex::Count> TexGLFormats =
	{
		ETexGLFormat::RGB_32F
	};

	const TArray<ETexFormat::Value, EGBufferTex::Count> TexFormats =
	{
		ETexFormat::RGB
	};

	const TArray<ETexWrap::Value, EGBufferTex::Count> TexWrapParams =
	{
		ETexWrap::ClampToEdge
	};

	const TArray<ETexMinFilter::Value, EGBufferTex::Count> MinFilterParams =
	{
		ETexMinFilter::Nearest
	};

	const TArray<ETexMagFilter::Value, EGBufferTex::Count> MagFilterParams =
	{
		ETexMagFilter::Nearest
	};

	for (EGBufferTex::Type I = 0; I < EGBufferTex::Count; ++I)
	{
		F_GL(GL::BindTexture(ETexTarget::T2D, Textures[I]));
		F_GL(GL::TexImage2D(ETexTarget::T2D, 0, TexGLFormats[I], Width, Height, TexFormats[I], EType::Float, nullptr));

		F_GL(GL::TexParameteri(ETexTarget::T2D, ETexParameter::TextureWrapS, TexWrapParams[I]));
		F_GL(GL::TexParameteri(ETexTarget::T2D, ETexParameter::TextureWrapT, TexWrapParams[I]));

		F_GL(GL::TexParameteri(ETexTarget::T2D, ETexParameter::TextureMinFilter, MinFilterParams[I]));
		F_GL(GL::TexParameteri(ETexTarget::T2D, ETexParameter::TextureMagFilter, MagFilterParams[I]));

		F_GL(GL::FramebufferTexture2D(EGBuffer::DrawFramebuffer, ColorAttachments[I], ETexTarget::T2D, Textures[I], 0));
	}

	F_GL(GL::BindTexture(ETexTarget::T2D, DepthStencilTex));
	F_GL(GL::TexImage2D(
		ETexTarget::T2D, 0, ETexGLFormat::DepthStencil, Width, Height,
		ETexFormat::DepthStencil, EType::Float32_UInt24_8_Rev, nullptr));
	F_GL(GL::FramebufferTexture2D(EGBuffer::DrawFramebuffer, EAttachment::DepthStencil, ETexTarget::T2D, DepthStencilTex, 0));

	F_GL(const EGBufferStatus::Value FramebufferStatus = GL::CheckFramebufferStatus(EGBuffer::Framebuffer));

	if (FramebufferStatus != EGBufferStatus::Complete)
	{
		F_GFXLogError("Failed to create a framebuffer due to error: " << EGBufferStatus::ToString(FramebufferStatus));
		F_GL(GL::BindFramebuffer(EGBuffer::Framebuffer, 0));
		DeInit();
		return;
	}

	F_GL(GL::BindFramebuffer(EGBuffer::Framebuffer, 0));
	F_GFXLog("Created framebuffer.");
}

bool FGBuffer::IsValid() const
{
	const bool Result = Framebuffer != 0;
	return Result;
}

void FGBuffer::DeInit()
{
	if (Framebuffer)
	{
		F_GL(GL::DeleteFramebuffers(1, &Framebuffer));
		Framebuffer = 0;
	}

	for (EGBufferTex::Type I = 0; I < EGBufferTex::Count; ++I)
	{
		if (Textures[I])
		{
			F_GL(GL::DeleteTextures(1, &Textures[I]));
			Textures[I] = 0;
		}
	}

	if (DepthStencilTex)
	{
		F_GL(GL::DeleteTextures(1, &DepthStencilTex));
		DepthStencilTex = 0;
	}
}

void FGBuffer::EnableWrite()
{
	F_Assert(IsValid(), "This class must be valid");
	F_GL(GL::BindFramebuffer(EGBuffer::DrawFramebuffer, Framebuffer));
	F_GL(GL::DrawBuffers(EGBufferTex::Count, &ColorAttachments[0]));
}

void FGBuffer::DisableWrite()
{
	F_Assert(IsValid(), "This class must be valid");
	F_GL(GL::BindFramebuffer(EGBuffer::DrawFramebuffer, 0));
}

void FGBuffer::EnableRead()
{
	F_Assert(IsValid(), "This class must be valid");
	F_GL(GL::BindFramebuffer(EGBuffer::ReadFramebuffer, Framebuffer));
}

void FGBuffer::DisableRead()
{
	F_Assert(IsValid(), "This class must be valid");
	F_GL(GL::BindFramebuffer(EGBuffer::ReadFramebuffer, 0));
}

void FGBuffer::BindTexture(const EGBufferTex::Value Texture)
{
	F_Assert(IsValid(), "This class must be valid");
	F_GL(GL::BindTexture(ETexTarget::T2D, Textures[Texture]));
}
