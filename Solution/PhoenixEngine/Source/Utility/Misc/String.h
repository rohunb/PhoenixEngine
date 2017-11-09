#pragma once

#include <cctype>
#include <string>

#ifdef __linux__
#include <cstring>
#endif

#include "Utility/Misc/Primitives.h"

namespace Phoenix
{
	typedef std::string FString;
	typedef std::wstring FWString;

	using namespace std::literals::string_literals; //for operator ""s

	//	--------------------------------------------------------------------------------
	/*! \brief A namespace containing algorithms for FString and FChar*. */
	namespace NString
	{
		//	--------------------------------------------------------------------------------
		/*! \brief Extracts the file extension from a file or file directory.
		*
		*	@param
		*	@return
		*	@example
		*/
		FString ExtractExtension(const FString& File);

		//	--------------------------------------------------------------------------------
		/*! \brief Extracts the file name from the provided directory.
		*
		*	@param FileDirectory An instance of FString.
		*	@return
		*	@example ExtractFileName("Directory/File.txt")
		*/
		FString ExtractFileName(const FString& FileDirectory);

		//	--------------------------------------------------------------------------------
		/*! \brief Extracts the file name from the provided directory.
		*
		*	@param
		*	@return
		*	@example ExtractFileName("Directory/File.txt")
		*/
		const FChar* ExtractFileName(const FChar* const FileDirectory);

		//	--------------------------------------------------------------------------------
		/*! \brief Extracts the function name from the full name.
		*
		*	@param
		*	@return
		*	@example
		*/
		const FChar* ExtractFunctionName(const FChar* const FullName);

		//	--------------------------------------------------------------------------------
		/*! \brief Finds the last position of any of the given characters.
		*
		*	@param Str A pointer to a null terminated string.
		*	@param StrLength The length of the Str parameter.
		*	@param Contents A pointer to a null terminated string of characters to look for.
		*	@return The position of the character that was found, or FString::npos if a
		*	suitable character was not found or the function failed.
		*	@example FindLastOf("Init/File.cpp", 13, "/\\")
		*/
		SizeT FindLastOf(
			const FChar* const String,
			const SizeT StringLength,
			const FChar* const Contents);

		//	--------------------------------------------------------------------------------
		/*! \brief Calculates the length of the string.
		*
		*	@param Str A pointer to a null terminated string.
		*	@return The number of characters up to but not including the null terminating
		*	character.
		*	@example GetLength("SomeString.txt")
		*/
		SizeT GetLength(const FChar* const String);

		//	--------------------------------------------------------------------------------
		/*! \brief 
		*
		*	@param
		*	@return
		*	@example
		*/
		bool HasExtension(const FChar* const File, const FChar* const Extension);

		//	--------------------------------------------------------------------------------
		/*! \brief Converts any lower case characters to lower case.
		*
		*	@param
		*	@return
		*	@example
		*/
		void ToLower(FString& String);

		//	--------------------------------------------------------------------------------
		/*! \brief Converts any lower case characters to upper case.
		*
		*	@param
		*	@return
		*	@example
		*/
		void ToLower(FChar* const String);
	}
}
