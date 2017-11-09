#pragma once

#include <SimpleINI/SimpleIni.h>

#include "Math/Vector2D.h"
#include "Utility/FileIO/File.h"
#include "Utility/Misc/String.h"

namespace Phoenix
{

	class FWindowConfig
	{
		friend class FEngineConfig;

	public:

		inline const FVector2D& GetDimensions() const
		{
			return Dimensions;
		}

		inline const FString& GetTitle() const
		{
			return Title;
		}

	private:

		FVector2D Dimensions;

		FString Title;
	};

	class FEngineConfig
	{

		typedef CSimpleIniA FINIParser;

	public:

		FEngineConfig();

		FEngineConfig(const FEngineConfig&&) = delete;
		
		~FEngineConfig();

		inline const FWindowConfig& GetWindowConfig() const
		{
			return WindowConfig;
		}

		bool Read();

	private:

		bool ParseWindowConfig();

		FFile ConfigFile;

		FINIParser INIParser;

		FWindowConfig WindowConfig;
	};

}
