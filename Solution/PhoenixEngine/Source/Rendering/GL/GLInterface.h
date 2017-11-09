#ifndef PHOENIX_GL_INTERFACE_H
#define PHOENIX_GL_INTERFACE_H

#include "Config/GFXCompileConfig.h"
#include "Rendering/Debug/GFXDebug.h"
#include "Rendering/GL/GLTypes.h"

namespace Phoenix
{
	namespace GL
	{
		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glActiveTexture.xml */
		void ActiveTexture(const ETex::Value Texture);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glAttachShader.xml */
		void AttachShader(const GLuint Program, const GLuint Shader);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindBuffer.xml */
		void BindBuffer(const EBuffer::Value Target, const GLuint Buffer);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindFramebuffer.xml */
		void BindFramebuffer(const EGBuffer::Value Target, const GLuint Framebuffer);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindTexture.xml */
		void BindTexture(const ETexTarget::Value Target, const GLuint Texture);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindVertexArray.xml */
		void BindVertexArray(const GLuint Array);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBlendFunc.xml */
		void BlendFunc(const EBlend::Value SFactor, const EBlend::Value DFactor);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBufferData.xml */
		void BufferData(
			const EBuffer::Value Target,
			const GLsizeiptr Size,
			const GLvoid* Data, 
			const EUsage::Value Usage);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBufferSubData.xml */
		void BufferSubData(
			const EBuffer::Value Target,
			const GLintptr Offset,
			const GLsizeiptr Size,
			const GLvoid* const Data);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCheckFramebufferStatus.xml */
		EGBufferStatus::Value CheckFramebufferStatus(const EGBuffer::Value Target);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClear.xml */
		void Clear(const EClearBit::Type ClearMask);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClearColor.xml */
		void ClearColor(const GLclampf Red, const GLclampf Green, const GLclampf Blue, const GLclampf Alpha);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCompileShader.xml */
		void CompileShader(const GLuint Shader);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCreateProgram.xml */
		GLuint CreateProgram();

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCreateShader.xml */
		GLuint CreateShader(const EShader::Value ShaderType);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteBuffers.xml */
		void DeleteBuffers(const GLsizei N, const GLuint* const Buffers);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteFramebuffers.xml */
		void DeleteFramebuffers(const GLsizei N, const GLuint* const Framebuffers);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteProgram.xml */
		void DeleteProgram(const GLuint Program);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteShader.xml */
		void DeleteShader(const GLuint Shader);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteTextures.xml */
		void DeleteTextures(const GLsizei N, const GLuint* const Textures);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteVertexArrays.xml */
		void DeleteVertexArrays(const GLsizei N, const GLuint* const Arrays);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnable.xml */
		void Disable(const ECapability::Value Cap);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawArrays.xml */
		void DrawArrays(
			const EMode::Value Mode,
			const GLint First,
			const GLsizei Count);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawArraysInstanced.xml */
		void DrawArraysInstanced(
			const EMode::Value Mode,
			const GLint First,
			const GLsizei Count,
			const GLsizei PrimCount);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawBuffers.xml */
		void DrawBuffers(const GLsizei N, const EAttachment::Value* const Bufs);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawElements.xml */
		void DrawElements(
			const EMode::Value Mode,
			const GLsizei Count,
			const EType::Value Type,
			const GLvoid* const Indices);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnable.xml */
		void Enable(const ECapability::Value Cap);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnableVertexAttribArray.xml */
		void EnableVertexAttribArray(const GLuint Index);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glFramebufferTexture.xml */
		void FramebufferTexture2D(
			const EGBuffer::Value Target,
			const EAttachment::Value Attachment,
			const ETexTarget::Value TexTarget,
			const GLuint Texture,
			const GLint Level);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenerateMipmap.xml */
		void GenerateMipmap(const ETexTarget::Value Target);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenFramebuffers.xml */
		void GenFramebuffers(const GLsizei N, GLuint* const Textures);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenTextures.xml */
		void GenTextures(const GLsizei N, GLuint* const Textures);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetProgramInfoLog.xml */
		void GetProgramInfoLog(
			const GLuint Program,
			const GLsizei MaxLength,
			GLsizei* const Length,
			GLchar* const InfoLog);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenBuffers.xml */
		void GenBuffers(const GLsizei BufferCount, GLuint* const Buffers);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenVertexArrays.xml */
		void GenVertexArrays(const GLsizei N, GLuint* const Arrays);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetProgram.xml */
		void GetProgramiv(const GLuint Program, const EGetProgram::Value PName, GLint& Params);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetShaderInfoLog.xml */
		void GetShaderInfoLog(
			const GLuint Shader,
			GLchar* const InfoLog,
			const GLsizei MaxLength,
			GLsizei& WrittenLength);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetShader.xml */
		void GetShaderiv(const GLuint Shader, const EShaderData::Value ShaderData, GLint& OutResult);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetString.xml */
		const GLubyte* GetString(const EGLInfo::Value Name);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetUniformLocation.xml */
		GLint GetUniformLocation(const GLuint Program, const GLchar* const Name);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glLinkProgram.xml */
		void LinkProgram(const GLuint Program);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glPixelStore.xml */
		void PixelStoref(const EPixelStorage::Value PName, const GLfloat Param);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glPixelStore.xml */
		void PixelStorei(const EPixelStorage::Value PName, const GLint Param);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glShaderSource.xml */
		void ShaderSource(const GLuint Shader, const GLchar* Code);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexImage2D.xml */
		void TexImage2D(
			const ETexTarget::Value Target,
			const GLint Level,
			const ETexGLFormat::Value InternalFormat,
			const GLsizei Width,
			const GLsizei Height,
			const ETexFormat::Value Format,
			const EType::Value Type,
			const GLvoid* const Data);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml */
		void TexParameterf(
			const ETexTarget::Value Target,
			const ETexParameter::Value PName,
			const GLfloat Param);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml */
		void TexParameteri(
			const ETexTarget::Value Target,
			const ETexParameter::Value PName,
			const GLint Param);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform1f(const GLint Location, const GLfloat V0);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform2f(const GLint Location, const GLfloat V0, const GLfloat V1);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform3f(
			const GLint Location,
			const GLfloat V0,
			const GLfloat V1,
			const GLfloat V2);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform4f(
			const GLint Location,
			const GLfloat V0,
			const GLfloat V1,
			const GLfloat V2,
			const GLfloat V3);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform1i(const GLint Location, const GLint V0);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform2i(const GLint Location, const GLint V0, const GLint V1);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform3i(
			const GLint Location,
			const GLint V0,
			const GLint V1,
			const GLint V2);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform4i(
			const GLint Location,
			const GLint V0,
			const GLint V1,
			const GLint V2,
			const GLint V3);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform1fv(
			const GLint Location,
			const GLsizei Count,
			const GLfloat* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform2fv(
			const GLint Location,
			const GLsizei Count,
			const GLfloat* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform3fv(
			const GLint Location,
			const GLsizei Count,
			const GLfloat* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform4fv(
			const GLint Location,
			const GLsizei Count,
			const GLfloat* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform1iv(
			const GLint Location,
			const GLsizei Count,
			const GLint* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform2iv(
			const GLint Location,
			const GLsizei Count,
			const GLint* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform3iv(
			const GLint Location,
			const GLsizei Count,
			const GLint* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform4iv(
			const GLint Location,
			const GLsizei Count,
			const GLint* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform1uiv(
			const GLint Location,
			const GLsizei Count,
			const GLuint* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform2uiv(
			const GLint Location,
			const GLsizei Count,
			const GLuint* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform3uiv(
			const GLint Location,
			const GLsizei Count,
			const GLuint* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void Uniform4uiv(
			const GLint Location,
			const GLsizei Count,
			const GLuint* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void UniformMatrix2fv(
			const GLint Location,
			const GLsizei Count,
			const EBool::Value Transpose,
			const GLfloat* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void UniformMatrix3fv(
			const GLint Location,
			const GLsizei Count,
			const EBool::Value Transpose,
			const GLfloat* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml */
		void UniformMatrix4fv(
			const GLint Location,
			const GLsizei Count,
			const EBool::Value Transpose,
			const GLfloat* const Value);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUseProgram.xml */
		void UseProgram(const GLuint Program);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glValidateProgram.xml */
		void ValidateProgram(const GLuint Program);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glVertexAttribDivisor.xml */
		void VertexAttribDivisor(const GLuint Index, const GLuint Divisor);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glVertexAttribPointer.xml */
		void VertexAttribPointer(
			const GLuint Index,
			const GLint Size,
			const EType::Value Type,
			const EBool::Value Normalized,
			const GLsizei Stride,
			const GLvoid* const Pointer);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glVertexAttribPointer.xml */
		void VertexAttribIPointer(
			const GLuint Index,
			const GLint Size,
			const EType::Value Type,
			const GLsizei Stride,
			const GLvoid* const Pointer);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glVertexAttribPointer.xml */
		void VertexAttribLPointer(
			const GLuint Index,
			const GLint Size,
			const EType::Value Type,
			const GLsizei Stride,
			const GLvoid* const Pointer);

		/* Ref: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glViewport.xml */
		void Viewport(const GLint X, const GLint Y, const GLsizei Width, const GLsizei Height);
	}
}

#ifndef PHOENIX_GFX_DEBUG_GL_CALLS
#	if _DEBUG
#		define PHOENIX_GFX_DEBUG_GL_CALLS 1
#	else
#		define PHOENIX_GFX_DEBUG_GL_CALLS 0
#	endif
#endif

#if PHOENIX_GFX_DEBUG_GL_CALLS

#	define F_GLDisplayErrors()															\
		for (UInt32 I_ = 0; I_ < PHOENIX_GFX_DEBUG_GL_MAX_ERROR_MSGS_PER_CALL; ++I_)	\
		{																				\
			const GL::EError::Type GLError_ = glGetError();								\
																						\
			if (GLError_ == GL::EError::None)											\
			{																			\
				break;																	\
			}																			\
																						\
			const FChar* const ErrorStr_ = GL::EError::ToString(GLError_);				\
			const FChar* const Description_ = GL::EError::ToDescription(GLError_);		\
																						\
			F_Assert(ErrorStr_, "ErrorStr is null.");									\
			F_Assert(Description_, "Description is null.");								\
			F_GFXLogError("GL Error: " << ErrorStr_ << " - " << Description_);			\
		}

#	define F_GLIgnoreErrors()															\
		for (UInt32 I_ = 0; I_ < PHOENIX_GFX_DEBUG_GL_MAX_ERROR_MSGS_PER_CALL; ++I_)	\
		{																				\
			const GL::EError::Type GLError_ = glGetError();								\
																						\
			if (GLError_ == GL::EError::None)											\
			{																			\
				break;																	\
			}																			\
		}

#	define F_GL(GLCall)		\
	GLCall;					\
	F_GLDisplayErrors();

#else

#	define F_GLDisplayErrors()

#	define F_GLIgnoreErrors()

#	define F_GL(GLCall)		\
	GLCall;

#endif

#endif
