#ifndef PHOENIX_FILE_H
#define PHOENIX_FILE_H

#include "Utility/FileIO/FileStream.h"
#include "Utility/Misc/Memory.h"
#include "Utility/Misc/String.h"
#include "Utility/Misc/StringStream.h"

namespace Phoenix
{
	class FFile
	{

	public:

		struct EFileOpenMode
		{
			typedef int Type; // std::ios_base::openmode

			enum Value : Type
			{
				Append = std::ios_base::app,
				AtEnd = std::ios_base::ate,
				Binary = std::ios_base::binary,
				Read = std::ios_base::in,
				Write = std::ios_base::out,
				Truncate = std::ios_base::trunc,
				Closed = 0x800
			};
		};

		FFile() = default;

		FFile(const FString& InFilePath);

	public:

		// If setting file path manually, Read() must be called.
		void SetFilePath(const FString& InPath);

		bool Open(EFileOpenMode::Value FileOpenMode);

		void Close();

		bool Read();

		// Ensures the file meets standards of a file
		bool IsValid() const;

		// Simply 'resets' the file
		bool Overwrite();

		// Overwrites with content of Data
		template <typename T>
		void Overwrite(const T& Data)
		{
			if (IsValid())
			{
				if (bIsOpen)
				{
					FileStream << Data;
				}
				else
				{
					// Error
				}
			}
		}

		// Check for if the file has nothing in it, or didn't load properly
		bool Empty() const;

		// Appends
		template <typename T>
		void Write(const T& Data)
		{
			if (IsValid())
			{
				if (bIsOpen)
				{
					FileStream << Data;
				}
				else
				{
					// Error
				}
			}
		}

		FString& Content();

		template <typename T>
		inline void operator<<(const T& Data)
		{
			if (IsValid())
			{
				Write(Data);
			}
		}

	protected:

	private:

		EFileOpenMode::Value OpenState{ EFileOpenMode::Value::Closed };

		FFileStream FileStream;

		FString FilePath;

		FString Data;

		bool bIsOpen { false };

	};
}

#endif