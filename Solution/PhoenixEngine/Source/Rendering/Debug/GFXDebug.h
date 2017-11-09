#ifndef PHOENIX_GFX_DEBUG_H
#define PHOENIX_GFX_DEBUG_H

#include "Config/GFXCompileConfig.h"
#include "Utility/Debug/Debug.h"

#if PHOENIX_GFX_DEBUG_CALLS

#	define F_GFXLogError(Msg) F_LogError(Msg)
#	define F_GFXLogErrorIf(Expr, Msg) F_LogErrorIf(Expr, Msg)
#	define F_GFXLogWarning(Msg) F_LogWarning(Msg)
#	define F_GFXLogWarningIf(Expr, Msg) F_LogWarningIf(Expr, Msg)
#	define F_GFXLogTrace(Msg) F_LogTrace(Msg)			
#	define F_GFXLogTraceIf(Expr, Msg) F_LogTraceIf(Expr, Msg)	
#	define F_GFXLog(Msg) F_Log(Msg)
#	define F_GFXLogIf(Expr, Msg) F_LogIf(Expr, Msg)	

#else

#	define F_GFXLogError(Msg)
#	define F_GFXLogErrorIf(Expr, Msg)
#	define F_GFXLogWarning(Msg)
#	define F_GFXLogWarningIf(Expr, Msg)
#	define F_GFXLogTrace(Msg)			
#	define F_GFXLogTraceIf(Expr, Msg)	
#	define F_GFXLog(Msg)
#	define F_GFXLogIf(Expr, Msg)	

#endif

#endif
