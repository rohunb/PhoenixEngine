#include "Stdafx.h"
#include "Rendering/GL/GLTypes.h"

#include "Utility/Debug/Assert.h"

namespace Phoenix
{
	namespace GL
	{
		const FChar* EError::ToString(const EError::Type GLError)
		{
			switch (GLError)
			{
				case EError::None:
					return "GL_NO_ERROR";
				case EError::InvalidEnum:
					return "GL_INVALID_ENUM";
				case EError::InvalidValue:
					return "GL_INVALID_VALUE";
				case EError::InvalidOperation:
					return "GL_INVALID_OPERATION";
				case EError::InvalidFrameBufferOperation:
					return "GL_INVALID_FRAMEBUFFER_OPERATION";
				case EError::OutOfMemory:
					return "GL_OUT_OF_MEMORY";
				case EError::StackUnderflow:
					return "GL_STACK_UNDERFLOW";
				case EError::StackOverflow:
					return "GL_STACK_OVERFLOW";
				default:
					F_Assert(false, "Invalid error ID.");
					break;
			}

			F_Assert(false, "Invalid error handling.");
			return nullptr;
		}

		const FChar* EError::ToDescription(const EError::Type GLError)
		{
			switch (GLError)
			{
				case EError::None:
					return "No error has been recorded.";
				case EError::InvalidEnum:
					return "An unacceptable value is specified for an enumerated argument.";
				case EError::InvalidValue:
					return "A numeric argument is out of range.";
				case EError::InvalidOperation:
					return "The specified operation is not allowed in the current state.";
				case EError::InvalidFrameBufferOperation:
					return "The framebuffer object is not complete.";
				case EError::OutOfMemory:
					return "There is not enough memory left to execute the command.";
				case EError::StackUnderflow:
					return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
				case EError::StackOverflow:
					return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
				default:
					F_Assert(false, "Invalid error ID.");
					break;
			}

			F_Assert(false, "Invalid error handling.");
			return nullptr;
		}

		const FChar* EGBufferStatus::ToString(const EGBufferStatus::Type GBufferStatus)
		{
			switch (GBufferStatus)
			{
				case EGBufferStatus::Complete:
					return "GL_FRAMEBUFFER_COMPLETE";
				case EGBufferStatus::Undefined:
					return "GL_FRAMEBUFFER_UNDEFINED";
				case EGBufferStatus::IncompleteAttachment:
					return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
				case EGBufferStatus::IncompleteMissingAttachment:
					return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
				case EGBufferStatus::IncompleteDrawBuffer:
					return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
				case EGBufferStatus::IncompleteReadBuffer:
					return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
				case EGBufferStatus::Unsupported:
					return "GL_FRAMEBUFFER_UNSUPPORTED";
				case EGBufferStatus::IncompleteMultiSample:
					return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
				case EGBufferStatus::IncompleteLayerTargets:
					return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
				case EGBufferStatus::Error:
					return "ERROR";
				default:
					F_Assert(false, "Invalid GBufferStatus.");
					break;
			}

			F_Assert(false, "Invalid error handling.");
			return nullptr;
		}
	}
}
