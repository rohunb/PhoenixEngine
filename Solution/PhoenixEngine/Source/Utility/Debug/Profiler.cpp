#include "Stdafx.h"
#include "Utility/Debug/Profiler.h"

#include "Utility/Debug/Assert.h"
#include "Utility/Misc/Timer.h"

using namespace Phoenix;


FOutputStream& FProfilerEntry::Output(FOutputStream& LHS) const
{
	for (UInt32 I = 0; I < IndentCount; ++I)
	{
		LHS << " ";
	}

	const std::streamsize DecimalCount = 5;
	LHS << std::fixed << std::setprecision(DecimalCount);

	LHS << ".." << TimeTaken << "s. "
		<< NString::ExtractFunctionName(Function) << "() in "
		<< NString::ExtractFileName(File) << " @" << Line;

	if (Message)
	{
		LHS << ". " << Message;
	}

	return LHS;
}

void FProfiler::Reset()
{
	Entries.clear();
}

SizeT FProfiler::ReserveEntryIndex()
{
	const SizeT NextEntryIndex = Entries.size();
	Entries.emplace_back(FProfilerEntry());
	return NextEntryIndex;
}

void FProfiler::AddEntry(const FProfilerEntry& ProfilerEntry, const SizeT Index)
{
	Entries[Index] = ProfilerEntry;
}

const FProfilerEntriesT& FProfiler::GetEntries() const
{
	return Entries;
}

bool FProfiler::HasEntries() const
{
	const bool Result = !Entries.empty();
	return Result;
}

UInt32 FProfiler::PushIndentCount()
{
	return IndentCount++;
}

void FProfiler::PopIndentCount()
{
	F_Assert(IndentCount, "Invalid pop operation.");
	--IndentCount;
}

FProfilerEntryAdder::FProfilerEntryAdder(FProfiler& InProfiler)
	: Profiler(InProfiler)
{
	EntryIndex = Profiler.ReserveEntryIndex();
	StartTime = FHighResolutionTimer::GetTimeInSeconds();
	Entry.IndentCount = Profiler.PushIndentCount();
}

FProfilerEntry& FProfilerEntryAdder::GetProfilerEntry()
{
	return Entry;
}

FProfilerEntryAdder::~FProfilerEntryAdder()
{
	const Float32 EndTime = FHighResolutionTimer::GetTimeInSeconds();
	Entry.TimeTaken = EndTime - StartTime;
	Profiler.AddEntry(Entry, EntryIndex);
	Profiler.PopIndentCount();
}
