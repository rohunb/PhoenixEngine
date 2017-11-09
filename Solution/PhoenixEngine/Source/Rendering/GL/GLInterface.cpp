#include "Stdafx.h"
#include "Rendering/GL/GLInterface.h"

#include "Utility/Debug/Assert.h"

namespace Phoenix
{
	namespace GL
	{
		void ActiveTexture(const ETex::Value Texture)
		{
			glActiveTexture(Texture);
		}

		void AttachShader(const GLuint Program, const GLuint Shader)
		{
			glAttachShader(Program, Shader);
		}

		void BindBuffer(const EBuffer::Value Target, const GLuint Buffer)
		{
			glBindBuffer(Target, Buffer);
		}

		void BindFramebuffer(const EGBuffer::Value Target, const GLuint Framebuffer)
		{
			glBindFramebuffer(Target, Framebuffer);
		}

		void BindTexture(const ETexTarget::Value Target, const GLuint Texture)
		{
			glBindTexture(Target, Texture);
		}

		void BindVertexArray(const GLuint Array)
		{
			glBindVertexArray(Array);
		}

		void BlendFunc(const EBlend::Value SFactor, const EBlend::Value DFactor)
		{
			glBlendFunc(SFactor, DFactor);
		}

		void BufferData(
			const EBuffer::Value Target,
			const GLsizeiptr Size,
			const GLvoid* Data,
			const EUsage::Value Usage)
		{
			glBufferData(Target, Size, Data, Usage);
		}

		void BufferSubData(
			const EBuffer::Value Target,
			const GLintptr Offset,
			const GLsizeiptr Size,
			const GLvoid* const Data)
		{
			glBufferSubData(Target, Offset, Size, Data);
		}

		EGBufferStatus::Value CheckFramebufferStatus(const EGBuffer::Value Target)
		{
			const EGBufferStatus::Value Status = static_cast<EGBufferStatus::Value>(glCheckFramebufferStatus(Target));
			return Status;
		}

		void Clear(const EClearBit::Type ClearMask)
		{
			glClear(ClearMask);
		}

		void ClearColor(const GLclampf Red, const GLclampf Green, const GLclampf Blue, const GLclampf Alpha)
		{
			glClearColor(Red, Green, Blue, Alpha);
		}

		void CompileShader(const GLuint Shader)
		{
			glCompileShader(Shader);
		}

		GLuint CreateProgram()
		{
			const GLuint Program = glCreateProgram();
			return Program;
		}

		GLuint CreateShader(const EShader::Value ShaderType)
		{
			const GLuint Shader = glCreateShader(ShaderType);
			return Shader;
		}

		void DeleteBuffers(const GLsizei N, const GLuint* const Buffers)
		{
			glDeleteBuffers(N, Buffers);
		}

		void DeleteFramebuffers(const GLsizei N, const GLuint* const Framebuffers)
		{
			glDeleteFramebuffers(N, Framebuffers);
		}

		void DeleteProgram(const GLuint Program)
		{
			glDeleteProgram(Program);
		}

		void DeleteShader(const GLuint Shader)
		{
			glDeleteShader(Shader);
		}

		void DeleteTextures(const GLsizei N, const GLuint* const Textures)
		{
			glDeleteTextures(N, Textures);
		}

		void DeleteVertexArrays(const GLsizei N, const GLuint* const Arrays)
		{
			glDeleteVertexArrays(N, Arrays);
		}

		void Disable(const ECapability::Value Cap)
		{
			glDisable(Cap);
		}

		void DrawArrays(
			const EMode::Value Mode,
			const GLint First,
			const GLsizei Count)
		{
			glDrawArrays(Mode, First, Count);
		}

		void DrawArraysInstanced(
			const EMode::Value Mode,
			const GLint First,
			const GLsizei Count,
			const GLsizei PrimCount)
		{
			glDrawArraysInstanced(Mode, First, Count, PrimCount);
		}

		void DrawBuffers(const GLsizei N, const EAttachment::Value* const Bufs)
		{
			static_assert(sizeof(EAttachment::Value) == sizeof(GLenum), "A type mismatch occurred.");
			const GLenum* const GLEnumBufs = reinterpret_cast<const GLenum*>(Bufs);
			glDrawBuffers(N, GLEnumBufs);
		}

		void DrawElements(
			const EMode::Value Mode,
			const GLsizei Count,
			const EType::Value Type,
			const GLvoid* const Indices)
		{
			glDrawElements(Mode, Count, Type, Indices);
		}

		void Enable(const ECapability::Value Cap)
		{
			glEnable(Cap);
		}

		void EnableVertexAttribArray(const GLuint Index)
		{
			glEnableVertexAttribArray(Index);
		}

		void FramebufferTexture2D(
			const EGBuffer::Value Target,
			const EAttachment::Value Attachment,
			const ETexTarget::Value TexTarget,
			const GLuint Texture,
			const GLint Level)
		{
			glFramebufferTexture2D(Target, Attachment, TexTarget, Texture, Level);
		}

		void GenerateMipmap(const ETexTarget::Value Target)
		{
			glGenerateMipmap(Target);
		}

		void GenFramebuffers(const GLsizei N, GLuint* const Textures)
		{
			glGenFramebuffers(N, Textures);
		}

		void GenTextures(const GLsizei N, GLuint* const Textures)
		{
			glGenTextures(N, Textures);
		}

		void GetProgramInfoLog(
			const GLuint Program,
			const GLsizei MaxLength,
			GLsizei* const Length,
			GLchar* const InfoLog)
		{
			glGetProgramInfoLog(Program, MaxLength, Length, InfoLog);
		}

		void GenBuffers(const GLsizei BufferCount, GLuint* const Buffers)
		{
			glGenBuffers(BufferCount, Buffers);
		}

		void GenVertexArrays(const GLsizei N, GLuint* const Arrays)
		{
			glGenVertexArrays(N, Arrays);
		}

		void GetProgramiv(const GLuint Program, const EGetProgram::Value PName, GLint& Params)
		{
			glGetProgramiv(Program, PName, &Params);
		}

		void GetShaderInfoLog(
			const GLuint Shader,
			GLchar* const InfoLog,
			const GLsizei MaxLength,
			GLsizei& WrittenLength)
		{
			F_Assert(InfoLog, "InfoLog must not be null.");
			F_Assert(WrittenLength == 0, "This value must be initialized 0 by default.");

			glGetShaderInfoLog(Shader, MaxLength, &WrittenLength, InfoLog);
		}

		void GetShaderiv(const GLuint Shader, const EShaderData::Value ShaderData, GLint& OutResult)
		{
			F_Assert(OutResult == 0, "This value must be initialized to 0 by default.");
			glGetShaderiv(Shader, ShaderData, &OutResult);
		}

		const GLubyte* GetString(const EGLInfo::Value Name)
		{
			const GLubyte* const Str = glGetString(Name);
			return Str;
		}

		GLint GetUniformLocation(const GLuint Program, const GLchar* const Name)
		{
			const GLint UniformLocation = glGetUniformLocation(Program, Name);
			return UniformLocation;
		}

		void LinkProgram(const GLuint Program)
		{
			glLinkProgram(Program);
		}

		void PixelStoref(const EPixelStorage::Value PName, const GLfloat Param)
		{
			glPixelStoref(PName, Param);
		}

		void PixelStorei(const EPixelStorage::Value PName, const GLint Param)
		{
			glPixelStorei(PName, Param);
		}

		void ShaderSource(const GLuint Shader, const GLchar* Code)
		{
			const GLsizei Count = 1;
			const GLint* const Length = nullptr;
			glShaderSource(Shader, Count, &Code, Length);
		}
		void TexImage2D(
			const ETexTarget::Value Target,
			const GLint Level,
			const ETexGLFormat::Value InternalFormat,
			const GLsizei Width,
			const GLsizei Height,
			const ETexFormat::Value Format,
			const EType::Value Type,
			const GLvoid* const Data)
		{
			glTexImage2D(Target, Level, InternalFormat, Width, Height, 0, Format, Type, Data);
		}

		void TexParameterf(
			const ETexTarget::Value Target,
			const ETexParameter::Value PName,
			const GLfloat Param)
		{
			glTexParameterf(Target, PName, Param);
		}

		void TexParameteri(
			const ETexTarget::Value Target,
			const ETexParameter::Value PName,
			const GLint Param)
		{
			glTexParameteri(Target, PName, Param);
		}

		void Uniform1f(const GLint Location, const GLfloat V0)
		{
			glUniform1f(Location, V0);
		}

		void Uniform2f(const GLint Location, const GLfloat V0, const GLfloat V1)
		{
			glUniform2f(Location, V0, V1);
		}

		void Uniform3f(
			const GLint Location,
			const GLfloat V0,
			const GLfloat V1,
			const GLfloat V2)
		{
			glUniform3f(Location, V0, V1, V2);
		}

		void Uniform4f(
			const GLint Location,
			const GLfloat V0,
			const GLfloat V1,
			const GLfloat V2,
			const GLfloat V3)
		{
			glUniform4f(Location, V0, V1, V2, V3);
		}

		void Uniform1i(const GLint Location, const GLint V0)
		{
			glUniform1i(Location, V0);
		}

		void Uniform2i(const GLint Location, const GLint V0, const GLint V1)
		{
			glUniform2i(Location, V0, V1);
		}

		void Uniform3i(
			const GLint Location,
			const GLint V0,
			const GLint V1,
			const GLint V2)
		{
			glUniform3i(Location, V0, V1, V2);
		}

		void Uniform4i(
			const GLint Location,
			const GLint V0,
			const GLint V1,
			const GLint V2,
			const GLint V3)
		{
			glUniform4i(Location, V0, V1, V2, V3);
		}

		void Uniform1fv(
			const GLint Location,
			const GLsizei Count,
			const GLfloat* const Value)
		{
			glUniform1fv(Location, Count, Value);
		}

		void Uniform2fv(
			const GLint Location,
			const GLsizei Count,
			const GLfloat* const Value)
		{
			glUniform2fv(Location, Count, Value);
		}

		void Uniform3fv(
			const GLint Location,
			const GLsizei Count,
			const GLfloat* const Value)
		{
			glUniform3fv(Location, Count, Value);
		}

		void Uniform4fv(
			const GLint Location,
			const GLsizei Count,
			const GLfloat* const Value)
		{
			glUniform4fv(Location, Count, Value);
		}

		void Uniform1iv(
			const GLint Location,
			const GLsizei Count,
			const GLint* const Value)
		{
			glUniform1iv(Location, Count, Value);
		}

		void Uniform2iv(
			const GLint Location,
			const GLsizei Count,
			const GLint* const Value)
		{
			glUniform2iv(Location, Count, Value);
		}

		void Uniform3iv(
			const GLint Location,
			const GLsizei Count,
			const GLint* const Value)
		{
			glUniform3iv(Location, Count, Value);
		}

		void Uniform4iv(
			const GLint Location,
			const GLsizei Count,
			const GLint* const Value)
		{
			glUniform4iv(Location, Count, Value);
		}

		void Uniform1uiv(
			const GLint Location,
			const GLsizei Count,
			const GLuint* const Value)
		{
			glUniform1uiv(Location, Count, Value);
		}

		void Uniform2uiv(
			const GLint Location,
			const GLsizei Count,
			const GLuint* const Value)
		{
			glUniform2uiv(Location, Count, Value);
		}

		void Uniform3uiv(
			const GLint Location,
			const GLsizei Count,
			const GLuint* const Value)
		{
			glUniform3uiv(Location, Count, Value);
		}

		void Uniform4uiv(
			const GLint Location,
			const GLsizei Count,
			const GLuint* const Value)
		{
			glUniform4uiv(Location, Count, Value);
		}

		void UniformMatrix2fv(
			const GLint Location,
			const GLsizei Count,
			const EBool::Value Transpose,
			const GLfloat* const Value)
		{
			glUniformMatrix2fv(Location, Count, Transpose, Value);
		}

		void UniformMatrix3fv(
			const GLint Location,
			const GLsizei Count,
			const EBool::Value Transpose,
			const GLfloat* const Value)
		{
			glUniformMatrix3fv(Location, Count, Transpose, Value);
		}

		void UniformMatrix4fv(
			const GLint Location,
			const GLsizei Count,
			const EBool::Value Transpose,
			const GLfloat* const Value)
		{
			glUniformMatrix4fv(Location, Count, Transpose, Value);
		}

		void UseProgram(const GLuint Program)
		{
			glUseProgram(Program);
		}

		void ValidateProgram(const GLuint Program)
		{
			glValidateProgram(Program);
		}

		void VertexAttribDivisor(const GLuint Index, const GLuint Divisor)
		{
			glVertexAttribDivisor(Index, Divisor);
		}

		void VertexAttribPointer(
			const GLuint Index,
			const GLint Size,
			const EType::Value Type,
			const EBool::Value Normalized,
			const GLsizei Stride,
			const GLvoid* const Pointer)
		{
			glVertexAttribPointer(Index, Size, Type, Normalized, Stride, Pointer);
		}

		void VertexAttribIPointer(
			const GLuint Index,
			const GLint Size,
			const EType::Value Type,
			const GLsizei Stride,
			const GLvoid* const Pointer)
		{
			glVertexAttribIPointer(Index, Size, Type, Stride, Pointer);
		}

		void VertexAttribLPointer(
			const GLuint Index,
			const GLint Size,
			const EType::Value Type,
			const GLsizei Stride,
			const GLvoid* const Pointer)
		{
			glVertexAttribLPointer(Index, Size, Type, Stride, Pointer);
		}

		void Viewport(const GLint X, const GLint Y, const GLsizei Width, const GLsizei Height)
		{
			glViewport(X, Y, Width, Height);
		}
	}
}
