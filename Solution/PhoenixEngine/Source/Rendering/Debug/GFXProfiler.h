#ifndef PHOENIX_GFX_PROFILER_H
#define PHOENIX_GFX_PROFILER_H

#include "Config/GFXCompileConfig.h"
#include "Utility/Debug/Profiler.h"

#if PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
namespace Phoenix
{
	struct FGFXProfiler
	{
		F_AddStaticObjectToClass(FProfiler);
	};
}
#	define F_GFXGetProfiler() FGFXProfiler::GetStaticObject()
#	define PHOENIX_GFX_MANAGES_A_PROFILER 1
#else
#	define F_GFXGetProfiler() F_GetProfiler()
#	define PHOENIX_GFX_MANAGES_A_PROFILER 0
#endif

#if PHOENIX_GFX_ENABLE_PROFILER

#	if !PHOENIX_UTILS_ENABLE_PROFILER
#		pragma message("Warning: PHOENIX_GFX_ENABLE_PROFILER is enabled but will have no effect due to PHOENIX_UTILS_ENABLE_PROFILER being disabled.")
#	endif

#	define F_GFXProfile() F_ProfileCustom(F_GFXGetProfiler())
#	define F_GFXProfileWithMsg(Msg) F_ProfileWithMsgCustom(F_GFXGetProfiler(), Msg)
#	define F_GFXResetProfiler() F_ResetProfilerCustom(F_GFXGetProfiler())
#else
#	define F_GFXProfile()
#	define F_GFXProfileWithMsg(Msg)
#	define F_GFXResetProfiler()
#endif

#endif