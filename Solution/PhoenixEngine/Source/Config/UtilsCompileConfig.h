#ifndef PHOENIX_UTILS_COMPILE_CONFIG_H
#define PHOENIX_UTILS_COMPILE_CONFIG_H

// This flag is used to determine whether or not the profiling macros will be enabled.
#ifndef PHOENIX_UTILS_ENABLE_PROFILER
#	define PHOENIX_UTILS_ENABLE_PROFILER 1
#endif

#pragma region EngineConfigVars

// #FIXME - This path shouldn't be relative. 
#define PHOENIX_ENGINE_CONFIG_PATH "../../PhoenixEngine/Config/EngineConfig.ini"
#define CONFIG_INI_WINDOW_SETTINGS_SECTION "WindowSettings"
#define CONFIG_INI_WINDOW_WIDTH_KEY "WindowWidth"
#define CONFIG_INI_WINDOW_HEIGHT_KEY "WindowHeight"
#define CONFIG_INI_WINDOW_TITLE_KEY "WindowTitle"

#pragma endregion

#endif
