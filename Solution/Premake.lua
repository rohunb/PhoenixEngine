workspace "PhoenixEngine"
	configurations { "Debug", "Release" }
	platforms{ "x32", "x64" }
	
	includedirs { "PhoenixEngine/Source", "Game/Source", "PhoenixTests/Source", "PhoenixBuild/Source", "Libraries/Include/", "Libraries/Include/FBX/", "Libraries/Include/GLM/" }
	libdirs {
		"Libraries/Lib/%{cfg.buildcfg}.%{cfg.platform}/**",
		"/usr/local/lib",
		os.findlib("glfw3"),
		os.findlib("glew")
	}
	sysincludedirs{"../Libraries/Include/**", "Libraries/Include/**"}
	links { "glfw3", "irrklang" }
	
	configuration "vs2015"
		links { "glew32s", "opengl32", "libfbxsdk-md", "assimp", "soil", "BulletCollision", "BulletDynamics", "LinearMath", "FreeType" }
	
	configuration "gmake"
		links { "glew", "opengl", "assimp", "soil" }
		buildoptions { "-I ../Libraries/Include/**" }
		includedirs { "Libraries/Include/**"}
		libdirs {
			"Libraries/Lib/%{cfg.buildcfg}/**",
		}
		
	configuration "xcode4"
		links { "glew", "OpenGL.framework", "soil", "assimp", "fbxsdk", "BulletCollision", "BulletDynamics", "LinearMath" }
		libdirs {
			"Libraries/Lib/%{cfg.buildcfg}/**",
		}
		xcodebuildsettings
		{
			["HEADER_SEARCH_PATHS"] = "../Libraries/Include/**"
		}
					
project "PhoenixEngine"
	kind "StaticLib"
	language "C++"
	location "PhoenixEngine"
	targetdir "Build/%{cfg.buildcfg}"
	sysincludedirs{"../Libraries/Include/**", "Libraries/Include/**"}

	filter "action:vs*"
		pchheader "Stdafx.h"
		pchsource "../Solution/PhoenixEngine/Source/Stdafx.cpp"

	filter "action:xcode*"
		-- Disable precompiled header use until Mitch can confirm it works on mac.
		--pchheader "Source/Stdafx.h"
		--pchsource "../Solution/PhoenixEngine/Source/Stdafx.cpp"
		
	filter "action:gmake*"
		-- Disable precompiled header use until the build is compiling successfully again.
	
	filter {}

	files { 
		"Assets/Shaders/**.txt",
		"PhoenixEngine/Source/**.h", 
		"PhoenixEngine/Source/**.hpp",
		"PhoenixEngine/Source/**.cpp", 
		"PhoenixEngine/Source/**.txt"
	}
	
	vpaths { ["Sources/*"] = { 
		"PhoenixEngine/Source/**.h", 
		"PhoenixEngine/Source/**.hpp", 
		"PhoenixEngine/Source/**.cpp", 
		"PhoenixEngine/Source/**.txt" } }

	filter "configurations:Debug"
		targetdir "Build/Debug"
		debugdir "Build/Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	filter "configurations:Release"
		targetdir "Build/Release"
		debugdir "Build/Release"
		defines { "NDEBUG" }
		optimize "On"

	configuration "gmake"
		linkoptions  { "-std=c++1y" }
		buildoptions { "-std=c++1y" }

	configuration "xcode4"
		linkoptions  { "-std=c++1y", "-stdlib=libc++" }
		buildoptions { "-std=c++1y", "-stdlib=libc++" }

project "Game"
	kind "ConsoleApp"
	language "C++"
	location "Game"
	targetdir "Build/%{cfg.buildcfg}"
	links { "PhoenixEngine" }
	sysincludedirs{"../Libraries/Include/**", "Libraries/Include/**"}

	files { 
		"Game/Source/**.h", 
		"Game/Source/**.hpp", 
		"Game/Source/**.cpp", 
		"Game/Source/**.txt" 
	}
	vpaths {
		["Sources/*"] = { 
			"Game/Source/**.h", 
			"Game/Source/**.hpp", 
			"Game/Source/**.cpp", 
			"Game/Source/**.txt"
		}
	}

	filter "configurations:Debug"
		targetdir "Build/Debug"
		debugdir "Build/Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	filter "configurations:Release"
		targetdir "Build/Release"
		debugdir "Build/Release"
		defines { "NDEBUG" }
		optimize "On"

	configuration "gmake"
		linkoptions  { "-std=c++1y" }
		buildoptions { "-std=c++1y" }

	configuration "xcode4"
		linkoptions  { "-std=c++1y", "-stdlib=libc++" }
		buildoptions { "-std=c++1y", "-stdlib=libc++" }


project "PhoenixTests"
	kind "ConsoleApp"
	language "C++"
	location "PhoenixTests"
	targetdir "Build/%{cfg.buildcfg}"
	links { "PhoenixEngine" }

	files { 
		"PhoenixTests/Source/**.h", 
		"PhoenixTests/Source/**.hpp", 
		"PhoenixTests/Source/**.cpp", 
		"PhoenixTests/Source/**.txt" 
	}
	vpaths {
		["Sources/*"] = { 
			"PhoenixTests/Source/**.h", 
			"PhoenixTests/Source/**.hpp", 
			"PhoenixTests/Source/**.cpp", 
			"PhoenixTests/Source/**.txt"
		}
	}

	filter "configurations:Debug"
		targetdir "Build/Debug"
		debugdir "Build/Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	filter "configurations:Release"
		targetdir "Build/Release"
		debugdir "Build/Release"
		defines { "NDEBUG" }
		optimize "On"

	configuration "gmake"
		linkoptions  { "-std=c++1y" }
		buildoptions { "-std=c++1y" }

	configuration "xcode4"
		linkoptions  { "-std=c++1y", "-stdlib=libc++" }
		buildoptions { "-std=c++1y", "-stdlib=libc++" }

project "PhoenixBuild"
	kind "ConsoleApp"
	language "C++"
	location "PhoenixBuild"
	targetdir "Build/%{cfg.buildcfg}"
	links { "PhoenixEngine" }
	sysincludedirs{"../Libraries/Include/**", "Libraries/Include/**"}

	files { 
		"PhoenixBuild/Source/**.h", 
		"PhoenixBuild/Source/**.hpp", 
		"PhoenixBuild/Source/**.cpp", 
		"PhoenixBuild/Source/**.txt" 
	}
	vpaths {
		["Sources/*"] = { 
			"PhoenixBuild/Source/**.h", 
			"PhoenixBuild/Source/**.hpp", 
			"PhoenixBuild/Source/**.cpp", 
			"PhoenixBuild/Source/**.txt"
		}
	}

	filter "configurations:Debug"
		targetdir "Build/Debug"
		debugdir "Build/Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	filter "configurations:Release"
		targetdir "Build/Release"
		debugdir "Build/Release"
		defines { "NDEBUG" }
		optimize "On"

	configuration "gmake"
		linkoptions  { "-std=c++1y" }
		buildoptions { "-std=c++1y" }

	configuration "xcode4"
		linkoptions  { "-std=c++1y", "-stdlib=libc++" }
		buildoptions { "-std=c++1y", "-stdlib=libc++" }
