#ifndef PHOENIX_PROFILER_H
#define PHOENIX_PROFILER_H

#include <iomanip>

#include "Config/UtilsCompileConfig.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Misc/OutputStream.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/StaticObject.h"
#include "Utility/Misc/String.h"

namespace Phoenix
{
	struct FProfilerEntry
	{
		const FChar* File{ nullptr };
		const FChar* Function{ nullptr };
		const FChar* Message{ nullptr };
		Int32 Line{ 0 };
		Float32 TimeTaken{ 0.f };
		UInt32 IndentCount{ 0 };

		FOutputStream& Output(FOutputStream& LHS) const;
	};

	static FOutputStream& operator<<(FOutputStream& LHS, const FProfilerEntry& RHS)
	{
		return RHS.Output(LHS);
	}

	typedef TVector<FProfilerEntry> FProfilerEntriesT;

	class FProfiler
	{
		F_AddStaticObjectToClass(FProfiler);
	public:
		FProfiler() = default;

		FProfiler(const FProfiler&) = delete;
		FProfiler& operator=(const FProfiler&) = delete;

		FProfiler(FProfiler&&) = delete;
		FProfiler& operator=(FProfiler&&) = delete;

		void Reset();

		SizeT ReserveEntryIndex();

		void AddEntry(const FProfilerEntry& ProfilerEntry, const SizeT Index);

		const FProfilerEntriesT& GetEntries() const;

		bool HasEntries() const;

		UInt32 PushIndentCount();

		void PopIndentCount();

	private:
		FProfilerEntriesT Entries;
		UInt32 IndentCount{ 0 };
	};

	static FOutputStream& operator<<(FOutputStream& LHS, const FProfiler& RHS)
	{
		const FProfilerEntriesT& Entries = RHS.GetEntries();

		LHS << "\n";
		for (const auto& Entry : Entries)
		{
			LHS << Entry << "\n";
		}

		return LHS;
	}

	class FProfilerEntryAdder
	{
	public:
		explicit FProfilerEntryAdder(FProfiler& InProfiler);

		FProfilerEntryAdder(const FProfilerEntryAdder&) = delete;
		FProfilerEntryAdder& operator=(const FProfilerEntryAdder&) = delete;

		FProfilerEntryAdder(FProfilerEntryAdder&&) = delete;
		FProfilerEntryAdder& operator=(FProfilerEntryAdder&&) = delete;

		~FProfilerEntryAdder();

		FProfilerEntry& GetProfilerEntry();

	private:
		FProfiler& Profiler;
		FProfilerEntry Entry;
		SizeT EntryIndex{ 0 };
		Float32 StartTime{ 0.f };
	};
}

#	define F_GetProfiler() FProfiler::GetStaticObject()

#if PHOENIX_UTILS_ENABLE_PROFILER

#	define F_ProfileCustom(Profiler)						\
	FProfilerEntryAdder PEAdder_(Profiler);					\
	FProfilerEntry& PEntry_ = PEAdder_.GetProfilerEntry();	\
	PEntry_.File = __FILE__;								\
	PEntry_.Function = __FUNCTION__;						\
	PEntry_.Line = __LINE__;

#	define F_Profile() F_ProfileCustom(F_GetProfiler())

#	define F_ProfileWithMsgCustom(Profiler, Msg) F_ProfileCustom(Profiler); PEntry_.Message = Msg;	
#	define F_ProfileWithMsg(Msg) F_ProfileWithMsgCustom(F_GetProfiler(), Msg)

#	define F_ResetProfilerCustom(Profiler) Profiler.Reset()
#	define F_ResetProfiler() F_ResetProfilerCustom(F_GetProfiler())

#else

#	define F_ProfileCustom(Profiler)
#	define F_Profile()
#	define F_ProfileWithMsgCustom(Profiler, Msg)
#	define F_ProfileWithMsg(Msg)
#	define F_ResetProfilerCustom(Profiler)
#	define F_ResetProfiler()

#endif

#endif
