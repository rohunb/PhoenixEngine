#pragma once

// This must always be defined.
#ifndef PHOENIX_GFX_COMPILE_CONFIG
#	define PHOENIX_GFX_COMPILE_CONFIG
#endif

// This flag is used to determine if debug values should be displayed.
#ifndef PHOENIX_GFX_DEBUG_CALLS
#	define PHOENIX_GFX_DEBUG_CALLS 1
#endif

// This flag is used to determine if each GL call will be tested for errors.  
// Set this to 0 for improved performance.
#ifndef PHOENIX_GFX_DEBUG_GL_CALLS
#	define PHOENIX_GFX_DEBUG_GL_CALLS 1
#endif

// This flag is used to determine the maximum number of errors that may be processed for each GL call.
#ifndef PHOENIX_GFX_DEBUG_GL_MAX_ERROR_MSGS_PER_CALL
#	define PHOENIX_GFX_DEBUG_GL_MAX_ERROR_MSGS_PER_CALL 10
#endif

// This flag is used to determine if supported opengl extensions are displayed on start up. 
#ifndef PHOENIX_GFX_DISPLAY_OPEN_GL_EXTENSIONS
#	define PHOENIX_GFX_DISPLAY_OPEN_GL_EXTENSIONS 1
#endif

// This flag is used to determine if some opengl and hardware information is displayed on start up.
#ifndef PHOENIX_GFX_DISPLAY_OPEN_GL_INFO
#	define PHOENIX_GFX_DISPLAY_OPEN_GL_INFO 1
#endif

// This flag is used to determine if graphics calls occur on the same thread when QueueRender is called,
// or on one or more threads that are handled by the gfx engine itself.
#ifndef PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING
#	define PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING 0
#endif

// This flag is used to determine if certain graphics calls are profiled.
#ifndef PHOENIX_GFX_ENABLE_PROFILER
#	define PHOENIX_GFX_ENABLE_PROFILER 1
#endif

// This flag is used to determine if profiling logging is enabled when PHOENIX_GFX_ENABLE_MULTI_THREADED_RENDERING is set to 1.
#ifndef PHOENIX_GFX_ENABLE_PROFILER_LOGGING_IN_MT_MODE
#	define PHOENIX_GFX_ENABLE_PROFILER_LOGGING_IN_MT_MODE 0
#endif

// This flag is used to determine if the max number of threads supported by the CPU should be created and used for loading.
#ifndef PHOENIX_GFX_USE_MAX_THREADS_FOR_LOADING
#	define PHOENIX_GFX_USE_MAX_THREADS_FOR_LOADING 1
#endif
