// #FIXME: Once all of the issues with the mac premake/linux build are sorted out,
// the PCH can be re-enabled for those platforms.
#ifdef _WIN32
#pragma message("Creating precompiled header...")

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cctype>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <ostream>
#include <random>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ExternalLib/AssimpIncludes.h"
#include "ExternalLib/BulletIncludes.h"
#include "ExternalLib/FBXIncludes.h"
#include "ExternalLib/FreeTypeIncludes.h"
#include "ExternalLib/GLEWIncludes.h"
#include "ExternalLib/GLFWIncludes.h"
#include "ExternalLib/GLIncludes.h"
#include "ExternalLib/GLMIncludes.h"
#include "ExternalLib/SOILIncludes.h"

#include "Utility/Debug/Assert.h"
#include "Utility/Misc/Primitives.h"

#endif
