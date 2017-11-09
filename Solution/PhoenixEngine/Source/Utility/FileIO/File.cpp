#include "Stdafx.h"

#include "Utility/Debug/Debug.h"
#include "Utility/FileIO/File.h"

namespace Phoenix
{

	FFile::FFile(const FString& InFilePath)
	{
		FilePath = InFilePath;

		Read();
	}

	void FFile::SetFilePath(const FString& InPath)
	{
		FilePath = InPath;
	}

	bool FFile::Open(EFileOpenMode::Value FileOpenMode)
	{
		if (IsValid())
		{
			if (bIsOpen)
			{
				Close();
			}
	
			FileStream.open(FilePath.c_str(), FileOpenMode);
	
			bIsOpen = FileStream.is_open();
			if (!bIsOpen)
			{
				F_LogError("Error opening " + FilePath);
				return false;
			}
	
			OpenState = FileOpenMode;
	
			return true;
		}
		else
		{
			return false;
		}
	}

	void FFile::Close()
	{
		if (bIsOpen)
		{
			FileStream.close();
			bIsOpen = FileStream.is_open();

			if (!bIsOpen)
			{
				OpenState = EFileOpenMode::Value::Closed;
			}
		}
	}

	bool FFile::Read()
	{
		bool bSuccess = false;

		if (IsValid())
		{
			Data.clear();

			bSuccess = Open(EFileOpenMode::Value::Read);
	
			if (bIsOpen)
			{
				FStringStream StringStream;
				StringStream << FileStream.rdbuf();
				Data = StringStream.str();
			}
		}

		if (!bSuccess)
		{
			F_LogError("Error reading file [" + FilePath + "]");
		}

		return bSuccess;
	}

	bool FFile::IsValid() const
	{
		// #FIXME - We should probably do some more checks on the string itself to 
		//			ensure that it's an actual filepath, but for now this is fine.
		bool bIsValid = !FilePath.empty();

		return bIsValid;
	}

	bool FFile::Empty() const
	{
		return Data.empty();
	}

	bool FFile::Overwrite()
	{
		bool bSuccess = false;

		if (IsValid())
		{
			if (bIsOpen)
			{
				Close();
			}
			
			bSuccess = Open(EFileOpenMode::Write);
		}

		return bSuccess;
	}

	FString& FFile::Content()
	{
		return Data;
	}

}