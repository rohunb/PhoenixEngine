#include "Stdafx.h"
#include "Rendering/Shader/Shader.h"

#include "Utility/Debug/Assert.h"
#include "Math/MatrixTransform.h"
#include "Rendering/Debug/GFXDebug.h"
#include "Rendering/Debug/GFXProfiler.h"
#include "Rendering/Shader/ShaderUniformNames.h"

using namespace Phoenix;
using namespace Phoenix::GL;

const GLint FShader::InvalidUniform = -1;

FShader::FInitParams::FInitParams()
{
	CodePtrArray.fill(nullptr);
}

FShader::FShader()
{
	Shaders.fill(0);
}

FShader::FShader(FShader&& RHS)
	: Program(RHS.Program)
	, Shaders(RHS.Shaders)
{
	RHS.PostMoveReset();
}

FShader& FShader::operator=(FShader&& RHS)
{
	if (this == &RHS)
	{
		return *this;
	}

	Program = RHS.Program;
	Shaders = RHS.Shaders;

	RHS.PostMoveReset();
	return *this;
}

FShader::~FShader()
{
	DeInit();
}

void FShader::Init(const FInitParams& InitParams)
{
	F_GFXProfile();
	DeInit();

	InitProgram();

	for (UInt32 I = 0; I < InitParams.CodePtrArray.size(); ++I)
	{
		if (!InitParams.CodePtrArray[I])
		{
			continue;
		}

		const EShaderIndex::Value Index = static_cast<EShaderIndex::Value>(I);
		InitShader(Index, InitParams.CodePtrArray[I]);

		if (!IsShaderValid(Index))
		{
			F_GFXLogError("Shader type " << Index << " is invalid.");
			DeInit();
			return;
		}
	}

	OnInitComplete();
}

bool FShader::IsValid() const
{
	const bool Result = IsProgramValid();
	return Result;
}

void FShader::DeInit()
{
	for (UInt32 I = 0; I < Shaders.size(); ++I)
	{
		const EShaderIndex::Value Index = static_cast<EShaderIndex::Value>(I);
		DeInitShader(Index);
	}

	DeInitProgram();
}

void FShader::Enable()
{
	F_Assert(IsValid(), "Shader is invalid.");
	F_GL(GL::UseProgram(Program));
}

void FShader::Disable()
{
	F_GL(GL::UseProgram(0));
}

GL::LocationT FShader::GetUniform(const GLchar* const Name) const
{
	F_Assert(Name, "UniformName is null.");

	F_GL(const GLint Location = GL::GetUniformLocation(Program, Name));
	F_GFXLogErrorIf(Location == FShader::InvalidUniform, "Failed to get valid uniform for " << Name);

	return Location;
}

void FShader::SetUniform(const GLint Integer, const GLchar* const Name)
{
	const GL::LocationT Location = GetUniform(Name);
	SetUniform(Integer, Location);
}

void FShader::SetUniform(const FMatrix3D& Matrix3D, const GLchar* const Name)
{
	const GL::LocationT Location = GetUniform(Name);
	SetUniform(Matrix3D, Location);
}

void FShader::SetUniform(const FMatrix4D& Matrix4D, const GLchar* const Name)
{
	const GL::LocationT Location = GetUniform(Name);
	SetUniform(Matrix4D, Location);
}

void FShader::SetUniform(const FVector2D& Vector2D, const GLchar* const Name)
{
	const GL::LocationT Location = GetUniform(Name);
	SetUniform(Vector2D, Location);
}

void FShader::SetUniform(const FVector3D& Vector3D, const GLchar* const Name)
{
	const GL::LocationT Location = GetUniform(Name);
	SetUniform(Vector3D, Location);
}

void FShader::SetUniform(const FVector4D& Vector4D, const GLchar* const Name)
{
	const GL::LocationT Location = GetUniform(Name);
	SetUniform(Vector4D, Location);
}

void FShader::SetUniform(const GLint Integer, const GL::LocationT Location)
{
	F_GL(GL::Uniform1i(Location, Integer));
}

void FShader::SetUniform(const FMatrix3D& Matrix3D, const GL::LocationT Location)
{
	const GLfloat* const Matrix3DPtr = glm::value_ptr(Matrix3D);

	F_GL(GL::UniformMatrix3fv(Location, 1, EBool::False, Matrix3DPtr));
}

void FShader::SetUniform(const FMatrix4D& Matrix4D, const GL::LocationT Location)
{
	const GLfloat* const Matrix4DPtr = glm::value_ptr(Matrix4D);

	F_GL(GL::UniformMatrix4fv(Location, 1, EBool::False, Matrix4DPtr));
}

void FShader::SetUniform(const FVector2D& Vector2D, const GL::LocationT Location)
{
	F_GL(GL::Uniform2f(Location, Vector2D.x, Vector2D.y));
}

void FShader::SetUniform(const FVector3D& Vector3D, const GL::LocationT Location)
{
	F_GL(GL::Uniform3f(Location, Vector3D.x, Vector3D.y, Vector3D.z));
}

void FShader::SetUniform(const FVector4D& Vector4D, const GL::LocationT Location)
{
	F_GL(GL::Uniform4f(Location, Vector4D.x, Vector4D.y, Vector4D.z, Vector4D.w));
}

void FShader::InitProgram()
{
	DeInitProgram();
	F_GL(Program = GL::CreateProgram());
	F_GFXLogErrorIf(Program == 0, "Failed to create shader program.");
}

bool FShader::IsProgramValid() const
{
	const bool Result = Program != 0;
	return Result;
}

void FShader::DeInitProgram()
{
	if (Program)
	{
		F_GL(GL::DeleteProgram(Program));
		Program = 0;
	}
}

void FShader::InitShader(const EShaderIndex::Value ShaderIndex, const GLchar* const Code)
{
	F_Assert(EShaderIndex::IsIndexValid(ShaderIndex), "Invalid index.");
	F_Assert(Code, "Code is invalid.");

	DeInitShader(ShaderIndex);
	GLuint& Shader = Shaders[ShaderIndex];

	const EShader::Value ShaderType = EShaderIndex::ToShader(ShaderIndex);
	F_GL(Shader = GL::CreateShader(ShaderType));

	if (Shader == 0)
	{
		const FChar* const ShaderTypeStrPtr = EShaderIndex::ToString(ShaderIndex);
		F_GFXLogError("Failed to create shader type " << ShaderTypeStrPtr << " with code " << Code);
		return;
	}

	F_GL(GL::ShaderSource(Shader, Code));
	F_GL(GL::CompileShader(Shader));

	GLint Success = 0;
	F_GL(GL::GetShaderiv(Shader, EShaderData::CompileStatus, Success));

	if (!Success)
	{
		GLchar InfoLog[1024] = "";
		GLsizei WrittenLength = 0;

		const FChar* const ShaderTypeStrPtr = EShaderIndex::ToString(ShaderIndex);
		F_GL(GL::GetShaderInfoLog(Shader, InfoLog, sizeof(InfoLog), WrittenLength));
		F_GFXLogError("Failed to compile shader type: " << ShaderTypeStrPtr << ". { " << InfoLog << " }");

		DeInitShader(ShaderIndex);
		return;
	}

	F_GL(GL::AttachShader(Program, Shader));
}

bool FShader::IsShaderValid(const EShaderIndex::Value ShaderType) const
{
	F_Assert(EShaderIndex::IsIndexValid(ShaderType), "Invalid index.");

	const bool Result = Shaders[ShaderType] != 0;
	return Result;
}

void FShader::DeInitShader(const EShaderIndex::Value ShaderIndex)
{
	F_Assert(EShaderIndex::IsIndexValid(ShaderIndex), "Invalid index.");

	if (Shaders[ShaderIndex])
	{
		F_GL(GL::DeleteShader(Shaders[ShaderIndex]));
		Shaders[ShaderIndex] = 0;
	}
}

bool FShader::IsShaderUniformValid(const GLchar* const UniformName) const
{
	F_Assert(UniformName, "UniformName is null.");

	const GLint Uniform = GetUniform(UniformName);
	const bool Result = Uniform != FShader::InvalidUniform;
	return Result;
}

void FShader::OnInitComplete()
{
	const auto OnErrorFunction = [this]()
	{
		GLchar InfoLog[1024] = "";
		F_GL(GL::GetProgramInfoLog(Program, sizeof(InfoLog), nullptr, InfoLog));
		F_GFXLogError("Error while finalizing shader. { " << InfoLog << " }");
		DeInit();
	};

	F_Assert(IsProgramValid(), "Program is invalid.");
	F_GL(GL::LinkProgram(Program));

	GLint ProgramSuccess = 0;
	F_GL(GL::GetProgramiv(Program, EGetProgram::LinkStatus, ProgramSuccess));

	if (!ProgramSuccess)
	{
		F_GFXLogError("Shader failed to link properly.  Error message follows.");
		OnErrorFunction();
		return;
	}

	ProgramSuccess = 0;
	F_GL(GL::ValidateProgram(Program));
	F_GL(GL::GetProgramiv(Program, EGetProgram::ValidateStatus, ProgramSuccess));

	if (!ProgramSuccess)
	{
		F_GFXLogError("Shader validation failed.  Error message follows.");
		OnErrorFunction();
		return;
	}

	F_GFXLog("Shader created.");
}

void FShader::PostMoveReset()
{
	Program = 0;
	Shaders.fill(0);
}
