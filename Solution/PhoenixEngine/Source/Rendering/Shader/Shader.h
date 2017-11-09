#ifndef PHOENIX_SHADER_H
#define PHOENIX_SHADER_H

#include "Utility/Containers/Array.h"
#include "Utility/Misc/String.h"
#include "Math/Matrix3D.h"
#include "Math/Matrix4D.h"
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Rendering/GL/GLInterface.h"

namespace Phoenix
{
	namespace EShaderIndex
	{
		typedef UInt32 Type;

		enum Value : Type
		{
			Compute,
			Vertex,
			TessControl,
			TessEvaluation,
			Geometry,
			Fragment,
			Count
		};

		static inline bool IsIndexValid(const EShaderIndex::Type ShaderIndex)
		{
			const bool Result = ShaderIndex < EShaderIndex::Count;
			return Result;
		}

		static inline GL::EShader::Value ToShader(const EShaderIndex::Value ShaderIndex)
		{
			static_assert(EShaderIndex::Count == 6, "This table requires updating.");
			static const TArray<GL::EShader::Value, EShaderIndex::Count> LookUpTable = {
				GL::EShader::Compute,
				GL::EShader::Vertex,
				GL::EShader::TessControl,
				GL::EShader::TessEvaluation,
				GL::EShader::Geometry,
				GL::EShader::Fragment,
			};

			const GL::EShader::Value Result = LookUpTable[ShaderIndex];
			return Result;
		}

		static const FChar* ToString(const EShaderIndex::Value ShaderIndex)
		{
			static_assert(EShaderIndex::Count == 6, "This table requires updating.");
			static const TArray<const GLchar* const, EShaderIndex::Count> LookUpTable = {
				"Compute",
				"Vertex",
				"TessControl",
				"TessEvaluation",
				"Geometry",
				"Fragment"
			};

			const FChar* const Result = LookUpTable[ShaderIndex];
			return Result;
		}
	}

	class FShader
	{
	public:
		typedef TArray<const GLchar*, EShaderIndex::Count> FCodePtrArray;
		typedef TArray<FString, EShaderIndex::Count> FCodeStrArray;

		static const GL::LocationT InvalidUniform;

		struct FInitParams
		{
			FCodePtrArray CodePtrArray;

			FInitParams();

			FInitParams(const FInitParams&) = delete;
			FInitParams& operator=(const FInitParams&) = delete;

			FInitParams(FInitParams&&) = delete;
			FInitParams& operator=(FInitParams&&) = delete;
		};

		FShader();

		FShader(const FShader&) = delete;
		FShader& operator=(const FShader&) = delete;

		FShader(FShader&&);
		FShader& operator=(FShader&&);

		~FShader();

		void Init(const FInitParams& InitParams);

		bool IsValid() const;

		void DeInit();

		void Enable();

		void Disable();

		GL::LocationT GetUniform(const GLchar* const Name) const;

		void SetUniform(const GLint Integer, const GLchar* const Name);
		void SetUniform(const FMatrix3D& Matrix3D, const GLchar* const Name);
		void SetUniform(const FMatrix4D& Matrix4D, const GLchar* const Name);
		void SetUniform(const FVector2D& Vector2D, const GLchar* const Name);
		void SetUniform(const FVector3D& Vector3D, const GLchar* const Name);
		void SetUniform(const FVector4D& Vector4D, const GLchar* const Name);

		static void SetUniform(const GLint Integer, const GL::LocationT Location);
		static void SetUniform(const FMatrix3D& Matrix3D, const GL::LocationT Location);
		static void SetUniform(const FMatrix4D& Matrix4D, const GL::LocationT Location);
		static void SetUniform(const FVector2D& Vector2D, const GL::LocationT Location);
		static void SetUniform(const FVector3D& Vector3D, const GL::LocationT Location);
		static void SetUniform(const FVector4D& Vector4D, const GL::LocationT Location);

	private:
		GL::ProgramT Program{ 0 };
		TArray<GL::ShaderT, EShaderIndex::Count> Shaders;

		void InitProgram();

		bool IsProgramValid() const;

		void DeInitProgram();

		void InitShader(const EShaderIndex::Value ShaderIndex, const GLchar* const Code);

		bool IsShaderValid(const EShaderIndex::Value ShaderIndex) const;

		void DeInitShader(const EShaderIndex::Value ShaderIndex);

		bool IsShaderUniformValid(const GLchar* const UniformName) const;

		void OnInitComplete();

		void PostMoveReset();
	};
}

#endif
