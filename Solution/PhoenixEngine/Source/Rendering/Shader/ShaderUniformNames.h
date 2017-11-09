#ifndef PHOENIX_SHADER_UNIFORM_NAMES_H
#define PHOENIX_SHADER_UNIFORM_NAMES_H

#include "ExternalLib/GLIncludes.h"

namespace Phoenix
{
	namespace EUniform
	{
		/* Matrices */
		static const GLchar* const Projection4D = "uProjection";
		static const GLchar* const View4D = "uView";
		static const GLchar* const World4D = "uWorld";
		static const GLchar* const InverseTransposeWorld3D = "uInverseTransposeWorld";
		static const GLchar* const Transform4D = "uTransform";

		/* Matrices (Combined) */
		static const GLchar* const WorldViewProjection4D = "uWorldViewProjection";

		/* Textures */
		static const GLchar* const DiffuseMap = "uDiffuseMap";
		static const GLchar* const NormalMap = "uNormalMap";

		/* Vectors */
		static const GLchar* const Color4D = "uColor";
		static const GLchar* const Flip2D = "uFlip";
		static const GLchar* const Position2D = "uPosition";
		static const GLchar* const Scale2D = "uScale";
		static const GLchar* const ViewRect2D = "uViewRect";
		static const GLchar* const Origin3D = "uOrigin";
	}
}

#endif
