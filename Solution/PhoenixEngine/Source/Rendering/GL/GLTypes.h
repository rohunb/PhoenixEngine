#ifndef PHOENIX_GL_TYPES_H
#define PHOENIX_GL_TYPES_H

#include "ExternalLib/GLIncludes.h"
#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	namespace GL
	{
		typedef GLuint ElementBufferT;
		typedef GLuint FrameBufferT;
		typedef GLuint ImageIDT;
		typedef GLsizei IndexCountT;
		typedef GLint LocationT;
		typedef GLint MipmapLevelT;
		typedef GLuint ProgramT;
		typedef GLuint ShaderT;
		typedef GLuint VertexArrayT;
		typedef GLuint VertexBufferT;

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glFramebufferTexture.xml */
		namespace EAttachment
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Color0 = GL_COLOR_ATTACHMENT0,
				Color1 = GL_COLOR_ATTACHMENT1,
				Color2 = GL_COLOR_ATTACHMENT2,
				Color3 = GL_COLOR_ATTACHMENT3,
				Color4 = GL_COLOR_ATTACHMENT4,

				Color5 = GL_COLOR_ATTACHMENT5,
				Color6 = GL_COLOR_ATTACHMENT6,
				Color7 = GL_COLOR_ATTACHMENT7,
				Color8 = GL_COLOR_ATTACHMENT8,
				Color9 = GL_COLOR_ATTACHMENT9,

				Color10 = GL_COLOR_ATTACHMENT10,
				Color11 = GL_COLOR_ATTACHMENT11,
				Color12 = GL_COLOR_ATTACHMENT12,
				Color13 = GL_COLOR_ATTACHMENT13,
				Color14 = GL_COLOR_ATTACHMENT14,

				Color15 = GL_COLOR_ATTACHMENT15,

				Depth = GL_DEPTH_ATTACHMENT,
				Stencil = GL_STENCIL_ATTACHMENT,
				DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBlendFunc.xml */
		namespace EBlend
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Zero = GL_ZERO,
				One = GL_ONE,

				SrcColor = GL_SRC_COLOR,
				OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
				DstColor = GL_DST_COLOR,
				OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,

				SrcAlpha = GL_SRC_ALPHA,
				OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
				DstAlpha = GL_DST_ALPHA,
				OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,

				ConstantColor = GL_CONSTANT_COLOR,
				OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
				ConstantAlpha = GL_CONSTANT_ALPHA,
				OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA
			};
		}

		namespace EBool
		{
			typedef GLenum Type;

			enum Value : Type
			{
				False = GL_FALSE,
				True = GL_TRUE
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindBuffer.xml */
		namespace EBuffer
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Array = GL_ARRAY_BUFFER,
				AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
				CopyRead = GL_COPY_READ_BUFFER,
				CopyWrite = GL_COPY_WRITE_BUFFER,
				DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,

				DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
				ElementArray = GL_ELEMENT_ARRAY_BUFFER,
				PixelPack = GL_PIXEL_PACK_BUFFER,
				PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
				Query = GL_QUERY_BUFFER,

				ShaderStorage = GL_SHADER_STORAGE_BUFFER,
				Texture = GL_TEXTURE_BUFFER,
				TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
				Uniform = GL_UNIFORM_BUFFER
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnable.xml */
		namespace ECapability
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Blend = GL_BLEND,
				ClipDistance0 = GL_CLIP_DISTANCE0,
				ClipDistance1 = GL_CLIP_DISTANCE1,
				ClipDistance2 = GL_CLIP_DISTANCE2,
				ClipDistance3 = GL_CLIP_DISTANCE3,

				ClipDistance4 = GL_CLIP_DISTANCE4,
				ClipDistance5 = GL_CLIP_DISTANCE5,
				ColorLogicOp = GL_COLOR_LOGIC_OP,
				CullFace = GL_CULL_FACE,
				DebugOutput = GL_DEBUG_OUTPUT,

				DebugOutputSynchronous = GL_DEBUG_OUTPUT_SYNCHRONOUS,
				DepthClamp = GL_DEPTH_CLAMP,
				DepthTest = GL_DEPTH_TEST,
				Dither = GL_DITHER,
				FramebufferSRGB = GL_FRAMEBUFFER_SRGB,

				LineSmooth = GL_LINE_SMOOTH,
				MultiSample = GL_MULTISAMPLE,
				PolygonOffsetFill = GL_POLYGON_OFFSET_FILL,
				PolygonOffsetLine = GL_POLYGON_OFFSET_LINE,
				PolygonOffsetPoint = GL_POLYGON_OFFSET_POINT,

				PolygonSmooth = GL_POLYGON_SMOOTH,
				PrimitiveRestart = GL_PRIMITIVE_RESTART,
				PrimitiveRestartFixedIndex = GL_PRIMITIVE_RESTART_FIXED_INDEX,
				RasterizerDiscard = GL_RASTERIZER_DISCARD,
				SampleAlphaToCoverage = GL_SAMPLE_ALPHA_TO_COVERAGE,

				SampleAlphaToOne = GL_SAMPLE_ALPHA_TO_ONE,
				SampleCoverage = GL_SAMPLE_COVERAGE,
				SampleShading = GL_SAMPLE_SHADING,
				SampleMask = GL_SAMPLE_MASK,
				ScissorTest = GL_SCISSOR_TEST,

				StencilTest = GL_STENCIL_TEST,
				TextureCubeMapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS,
				ProgramPointSize = GL_PROGRAM_POINT_SIZE
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClear.xml */
		namespace EClearBit
		{
			typedef GLbitfield Type;

			enum Mask : Type
			{
				Color = GL_COLOR_BUFFER_BIT,
				Depth = GL_DEPTH_BUFFER_BIT,
				Stencil = GL_STENCIL_BUFFER_BIT,
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml */
		namespace EError
		{
			typedef GLenum Type;

			enum Value : Type
			{
				None = GL_NO_ERROR,
				InvalidEnum = GL_INVALID_ENUM,
				InvalidValue = GL_INVALID_VALUE,
				InvalidOperation = GL_INVALID_OPERATION,
				InvalidFrameBufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
				OutOfMemory = GL_OUT_OF_MEMORY,
				StackUnderflow = GL_STACK_UNDERFLOW,
				StackOverflow = GL_STACK_OVERFLOW,
			};

			const FChar* ToString(const EError::Type GLError);

			const FChar* ToDescription(const EError::Type GLError);
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindFramebuffer.xml */
		namespace EGBuffer
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Framebuffer = GL_FRAMEBUFFER,
				DrawFramebuffer = GL_DRAW_FRAMEBUFFER,
				ReadFramebuffer = GL_READ_FRAMEBUFFER
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCheckFramebufferStatus.xml */
		namespace EGBufferStatus
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Complete = GL_FRAMEBUFFER_COMPLETE,
				Undefined = GL_FRAMEBUFFER_UNDEFINED,
				IncompleteAttachment = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
				IncompleteMissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
				IncompleteDrawBuffer = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,

				IncompleteReadBuffer = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
				Unsupported = GL_FRAMEBUFFER_UNSUPPORTED,
				IncompleteMultiSample = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
				IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,

				Error = 0
			};

			const FChar* ToString(const EGBufferStatus::Type GBufferStatus);
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetProgram.xml */
		namespace EGetProgram
		{
			typedef GLenum Type;

			enum Value : Type
			{
				DeleteStatus = GL_DELETE_STATUS,
				LinkStatus = GL_LINK_STATUS,
				ValidateStatus = GL_VALIDATE_STATUS,
				InfoLogLength = GL_INFO_LOG_LENGTH,
				AttachedShaders = GL_ATTACHED_SHADERS,

				ActiveAttributes = GL_ACTIVE_ATTRIBUTES,
				ActiveAttributeMaxLength = GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
				ActiveUniforms = GL_ACTIVE_UNIFORMS,
				ActiveUniformMaxLength = GL_ACTIVE_UNIFORM_MAX_LENGTH
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetString.xml */
		namespace EGLInfo
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Vendor = GL_VENDOR,
				Renderer = GL_RENDERER,
				Version = GL_VERSION,
				ShadingLanguageVersion = GL_SHADING_LANGUAGE_VERSION,
				Extensions = GL_EXTENSIONS
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawElements.xml */
		namespace EMode
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Points = GL_POINTS,
				LineStrip = GL_LINE_STRIP,
				LineLoop = GL_LINE_LOOP,
				Lines = GL_LINES,
				LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,

				LinesAdjacency = GL_LINES_ADJACENCY,
				TriangleStrip = GL_TRIANGLE_STRIP,
				TriangleFan = GL_TRIANGLE_FAN,
				Triangles = GL_TRIANGLES,
				TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,

				TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
				Patches = GL_PATCHES
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glPixelStore.xml */
		namespace EPixelStorage
		{
			typedef GLenum Type;

			enum Value : Type
			{
				PackSwapBytes = GL_PACK_SWAP_BYTES,
				PackLSBFirst = GL_PACK_LSB_FIRST,
				PackRowLength = GL_PACK_ROW_LENGTH,
				PackImageHeight = GL_PACK_IMAGE_HEIGHT,
				PackSkipPixels = GL_PACK_SKIP_PIXELS,
				PackSkipRows = GL_PACK_SKIP_ROWS,
				PackSkipImages = GL_PACK_SKIP_IMAGES,
				PackAlignment = GL_PACK_ALIGNMENT,

				UnpackSwapBytes = GL_UNPACK_SWAP_BYTES,
				UnpackLSBFirst = GL_UNPACK_LSB_FIRST,
				UnpackRowLength = GL_UNPACK_ROW_LENGTH,
				UnpackImageHeight = GL_UNPACK_IMAGE_HEIGHT,
				UnpackSkipPixels = GL_UNPACK_SKIP_PIXELS,
				UnpackSkipRows = GL_UNPACK_SKIP_ROWS,
				UnpackSkipImages = GL_UNPACK_SKIP_IMAGES,
				UnpackAlignment = GL_UNPACK_ALIGNMENT
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCreateShader.xml */
		namespace EShader
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Compute = GL_COMPUTE_SHADER,
				Vertex = GL_VERTEX_SHADER,
				TessControl = GL_TESS_CONTROL_SHADER,
				TessEvaluation = GL_TESS_EVALUATION_SHADER,
				Geometry = GL_GEOMETRY_SHADER,
				Fragment = GL_FRAGMENT_SHADER,
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetShader.xml */
		namespace EShaderData
		{
			typedef GLenum Type;

			enum Value : Type
			{
				ShaderType = GL_SHADER_TYPE,
				DeleteStatus = GL_DELETE_STATUS,
				CompileStatus = GL_COMPILE_STATUS,
				InfoLogLength = GL_INFO_LOG_LENGTH,
				SourceLength = GL_SHADER_SOURCE_LENGTH,
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glActiveTexture.xml */
		namespace ETex
		{
			typedef GLenum Type;

			enum Value : Type
			{
				T0 = GL_TEXTURE0,
				T1 = GL_TEXTURE1,
				T2 = GL_TEXTURE2,
				T3 = GL_TEXTURE3,
				T4 = GL_TEXTURE4,

				T5 = GL_TEXTURE5,
				T6 = GL_TEXTURE6,
				T7 = GL_TEXTURE7,
				T8 = GL_TEXTURE8,
				T9 = GL_TEXTURE9,

				T10 = GL_TEXTURE10,
				T11 = GL_TEXTURE11,
				T12 = GL_TEXTURE12,
				T13 = GL_TEXTURE13,
				T14 = GL_TEXTURE14,

				T15 = GL_TEXTURE15,
				T16 = GL_TEXTURE16,
				T17 = GL_TEXTURE17,
				T18 = GL_TEXTURE18,
				T19 = GL_TEXTURE19,

				T20 = GL_TEXTURE20,
				T21 = GL_TEXTURE21,
				T22 = GL_TEXTURE22,
				T23 = GL_TEXTURE23,
				T24 = GL_TEXTURE24,

				T25 = GL_TEXTURE25,
				T26 = GL_TEXTURE26,
				T27 = GL_TEXTURE27,
				T28 = GL_TEXTURE28,
				T29 = GL_TEXTURE29,

				T30 = GL_TEXTURE30,
				T31 = GL_TEXTURE31,
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexImage2D.xml */
		namespace ETexFormat
		{
			typedef GLenum Type;

			enum Value : Type
			{
				Red = GL_RED,
				RG = GL_RG,
				RGB = GL_RGB,
				BGR = GL_BGR,
				RGBA = GL_RGBA,

				BGRA = GL_BGRA,
				RedInteger = GL_RED_INTEGER,
				RGInteger = GL_RG_INTEGER,
				RGBInteger = GL_RGB_INTEGER,
				BGRInteger = GL_BGR_INTEGER,

				RGBAInteger = GL_RGBA_INTEGER,
				BGRAInteger = GL_BGRA_INTEGER,
				StencilIndex = GL_STENCIL_INDEX,
				DepthComponent = GL_DEPTH_COMPONENT,
				DepthStencil = GL_DEPTH_STENCIL
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexImage2D.xml */
		namespace ETexGLFormat
		{
			typedef GLint Type;

			enum Value : Type
			{
				/* Table 1: Base Internal Formats */
				DepthComponent = GL_DEPTH_COMPONENT,
				DepthStencil = GL_DEPTH_STENCIL,
				Red = GL_RED,
				RG = GL_RG,
				RGB = GL_RGB,
				RGBA = GL_RGBA,

				/* Table 2: Sized Internal Formats */
				// Note: Not all of the formats are represented here.
				R_32F = GL_R32F,
				RG_32F = GL_RG32F,
				RGB_32F = GL_RGB32F,
				RGBA_32F = GL_RGBA32F
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml */
		namespace ETexMagFilter
		{
			typedef GLint Type;

			enum Value : Type
			{
				Nearest = GL_NEAREST,
				Linear = GL_LINEAR,
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml */
		namespace ETexMinFilter
		{
			typedef GLint Type;

			enum Value : Type
			{
				Nearest = GL_NEAREST,
				Linear = GL_LINEAR,
				NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
				LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
				NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
				LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml */
		namespace ETexParameter
		{
			typedef GLenum Type;

			enum Value : Type
			{
				DepthStencilTextureMode = GL_DEPTH_STENCIL_TEXTURE_MODE,
				TextureBaseLevel = GL_TEXTURE_BASE_LEVEL,
				TextureCompareFunc = GL_TEXTURE_COMPARE_FUNC,
				TextureCompareMode = GL_TEXTURE_COMPARE_MODE,
				TextureLODBias = GL_TEXTURE_LOD_BIAS,

				TextureMinFilter = GL_TEXTURE_MIN_FILTER,
				TextureMagFilter = GL_TEXTURE_MAG_FILTER,
				TextureMinLOD = GL_TEXTURE_MIN_LOD,
				TextureMaxLOD = GL_TEXTURE_MAX_LOD,
				TextureMaxLevel = GL_TEXTURE_MAX_LEVEL,

				TextureSwizzleR = GL_TEXTURE_SWIZZLE_R,
				TextureSwizzleG = GL_TEXTURE_SWIZZLE_G,
				TextureSwizzleA = GL_TEXTURE_SWIZZLE_A,
				TextureWrapS = GL_TEXTURE_WRAP_S,
				TextureWrapT = GL_TEXTURE_WRAP_T,

				TextureWrapR = GL_TEXTURE_WRAP_R
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindTexture.xml */
		namespace ETexTarget
		{
			typedef GLenum Type;

			enum Value : Type
			{
				T1D = GL_TEXTURE_1D,
				T2D = GL_TEXTURE_2D,
				T3D = GL_TEXTURE_3D,
				Array1D = GL_TEXTURE_1D_ARRAY,
				Array2D = GL_TEXTURE_2D_ARRAY,

				Rectangle = GL_TEXTURE_RECTANGLE,
				CubeMap = GL_TEXTURE_CUBE_MAP,
				CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
				Buffer = GL_TEXTURE_BUFFER,
				MultiSample2D = GL_TEXTURE_2D_MULTISAMPLE,

				MultiSampleArray2D = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml */
		namespace ETexWrap
		{
			typedef GLint Type;

			enum Value : Type
			{
				ClampToEdge = GL_CLAMP_TO_EDGE,
				ClampToBorder = GL_CLAMP_TO_BORDER,
				MirroredRepeat = GL_MIRRORED_REPEAT,
				Repeat = GL_REPEAT,
				MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
			};
		}

		/* Refs: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glVertexAttribPointer.xml
		https://www.khronos.org/opengles/sdk/docs/man3/docbook4/xhtml/glTexImage2D.xml */
		namespace EType
		{
			typedef GLenum Type;

			enum Value
			{
				Byte = GL_BYTE,
				UByte = GL_UNSIGNED_BYTE,

				Short = GL_SHORT,
				UShort = GL_UNSIGNED_SHORT,

				Int = GL_INT,
				UInt = GL_UNSIGNED_INT,

				HalfFloat = GL_HALF_FLOAT,
				Float = GL_FLOAT,
				Double = GL_DOUBLE,

				UShort5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
				UShort4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
				UShort5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,

				Int2_10_10_10_Rev = GL_INT_2_10_10_10_REV,

				UInt24_8 = GL_UNSIGNED_INT_24_8,
				UInt2_10_10_10_Rev = GL_UNSIGNED_INT_2_10_10_10_REV,
				UInt5_9_9_9_Rev = GL_UNSIGNED_INT_5_9_9_9_REV,
				UInt10F_11F_11F_Rev = GL_UNSIGNED_INT_10F_11F_11F_REV,

				Float32_UInt24_8_Rev = GL_FLOAT_32_UNSIGNED_INT_24_8_REV,
			};
		}

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBufferData.xml */
		namespace EUsage
		{
			typedef GLenum Type;

			enum Value : Type
			{
				StreamDraw = GL_STREAM_DRAW,
				StreamRead = GL_STREAM_READ,
				StreamCopy = GL_STREAM_COPY,

				StaticDraw = GL_STATIC_DRAW,
				StaticRead = GL_STATIC_READ,
				StaticCopy = GL_STATIC_COPY,

				DynamicDraw = GL_DYNAMIC_DRAW,
				DynamicRead = GL_DYNAMIC_READ,
				DynamicCopy = GL_DYNAMIC_COPY
			};
		}
	}
}

#endif
