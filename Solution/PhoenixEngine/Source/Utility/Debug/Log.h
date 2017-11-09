#ifndef PHOENIX_LOG_H
#define PHOENIX_LOG_H

#include "Utility/Debug/Assert.h"
#include "Utility/FileIO/FileStream.h"
#include "Utility/Misc/StaticObject.h"
#include "Utility/Misc/String.h"
#include "Utility/Threading/Mutex.h"

namespace Phoenix
{
	class FLog
	{
		F_AddStaticObjectToClass(FLog);
	public:
		FLog() = default;

		FLog(const FLog&) = delete;
		FLog& operator=(const FLog&) = delete;

		FLog(FLog&&) = delete;
		FLog& operator=(FLog&&) = delete;

		~FLog();

		void Init(const FString& File);

		bool IsValid() const;

		void DeInit();

		FMutex& GetLock();

		template <class T>
		FLog& operator<<(const T& Data);

	protected:
	private:
		FOutputFileStream OFStream;
		FMutex Lock;
	};

	inline FLog::~FLog()
	{
		DeInit();
	}

	inline void FLog::Init(const FString& File)
	{
		DeInit();
		OFStream.open(File, std::ios_base::out | std::ios_base::trunc);
	}

	inline bool FLog::IsValid() const
	{
		const bool bIsValid = OFStream.is_open();
		return bIsValid;
	}

	inline void FLog::DeInit()
	{
		if (OFStream.is_open())
		{
			OFStream.close();
		}
	}

	inline FMutex& FLog::GetLock()
	{
		return Lock;
	}

	template <class T>
	FLog& FLog::operator<<(const T& Data)
	{
		F_Assert(IsValid(), "This class must be valid.");
		OFStream << Data;
		return *this;
	}

	template <>
	inline FLog& FLog::operator<<(const FConsoleColor& Data)
	{
		F_Assert(IsValid(), "This class must be valid.");
		return *this;
	}
}

#endif
