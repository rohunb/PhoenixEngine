#ifndef PHOENIX_WIN32_INCLUDES_H
#define PHOENIX_WIN32_INCLUDES_H

#if _WIN32

// Note: Ignore C4005 - APIENTRY is defined in other external header files.
#pragma warning(push)
#pragma warning(disable : 4005)
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#pragma warning(pop)

#endif

#endif