#include "Stdafx.h"

#include "EngineConfig.h"

#include "Config/UtilsCompileConfig.h"
#include "Utility/Misc/Algorithm.h"

namespace Phoenix
{

	FEngineConfig::FEngineConfig()
	{
		ConfigFile.SetFilePath(PHOENIX_ENGINE_CONFIG_PATH);
		
		bool bReadSuccess = ConfigFile.Read();
		if (!bReadSuccess)
		{
			// #FIXME - Fatal Error, stop engine || fall to defaults
		}

		INIParser.SetUnicode();
		INIParser.LoadData(ConfigFile.Content());
	}

	FEngineConfig::~FEngineConfig()
	{
	}

	bool FEngineConfig::Read()
	{
		bool bWindowParseSuccess = ParseWindowConfig();
		bool bParseSuccess = bWindowParseSuccess; // && OtherSuccess && OtherSuccess, etc

		ConfigFile.Close();

		return bParseSuccess;
	}

	bool FEngineConfig::ParseWindowConfig()
	{
		const int WindowX = IntFromString(INIParser.GetValue(CONFIG_INI_WINDOW_SETTINGS_SECTION, CONFIG_INI_WINDOW_WIDTH_KEY, "1024"));
		const int WindowY = IntFromString(INIParser.GetValue(CONFIG_INI_WINDOW_SETTINGS_SECTION, CONFIG_INI_WINDOW_HEIGHT_KEY, "768"));
		const FString WindowTitle = INIParser.GetValue(CONFIG_INI_WINDOW_SETTINGS_SECTION, CONFIG_INI_WINDOW_TITLE_KEY, "DefaultTitle");

		WindowConfig.Dimensions = FVector2D(WindowX, WindowY);
		WindowConfig.Title = WindowTitle;

		return true;
	}

}