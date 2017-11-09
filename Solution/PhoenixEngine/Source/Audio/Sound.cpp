#include "Stdafx.h"
#include "Sound.h"

#include "Utility/Debug/Assert.h"
#include "Utility/Debug/Debug.h"

using namespace Phoenix;

const Float32 FSound::MinAttenDistance = 1.f;

FSound::~FSound()
{
	DeInit();
}

void FSound::Init(irrklang::ISoundEngine* InSoundEngine, const FString& InFile)
{
	F_Assert(InSoundEngine, "InSoundEngine is null.");
	F_Assert(InFile.size(), "InFile is empty.");

	DeInit();

	SoundEngine = InSoundEngine;
	SoundEngine->grab();

	File = InFile;
}

bool FSound::IsValid() const
{
	const bool IsSoundEngineValid = SoundEngine != nullptr;
	const bool IsFileValid = File.size() != 0;

	const bool Result =
		IsSoundEngineValid &&
		IsFileValid;
	
	return Result;
}

void FSound::DeInit()
{
	File.clear();

	DropSound();

	if (SoundEngine)
	{
		SoundEngine->drop();
		SoundEngine = nullptr;
	}
}

void FSound::SetFile(const FString& InFile)
{
	F_Assert(IsValid(), "This class is invalid.");
	File = InFile;
}

void FSound::SetVolume(const Float32 Volume)
{
	F_Assert(IsValid(), "This class is in an invalid state.");
	F_LogErrorIf(!Sound, "SetVolume was called without a managed Sound.");

	if (Sound)
	{
		Sound->setVolume(Volume);
	}
}

const FString& FSound::GetFile() const
{
	return File;
}

Float32 FSound::GetVolume() const
{
	F_Assert(IsValid(), "This class is in an invalid state.");

	if (!Sound)
	{
		F_LogError("GetVolume was called without a managed Sound.");
		return 0.f;
	}

	const Float32 Volume = Sound->getVolume();
	return Volume;
}

void FSound::Play2D(const Float32 Volume, const bool LoopSound)
{
	F_Assert(IsValid(), "This class is in an invalid state.");
	irrklang::ISoundSource* SoundSource = GetSource();

	if (!SoundSource)
	{
		F_LogError("Failed to play a 2d sound.");
		return;
	}

	SoundSource->setDefaultVolume(Volume);
	
	const bool StartPaused = false;
	const bool TrackSound = true;

	DropSound();
	Sound = SoundEngine->play2D(SoundSource, LoopSound, StartPaused, TrackSound);
	F_LogErrorIf(!Sound, "TrackSound was specified but the returned pointer was null.");
}

void FSound::Play2DInstanced(const Float32 Volume)
{
	F_Assert(IsValid(), "This class is in an invalid state.");
	irrklang::ISoundSource* SoundSource = GetSource();

	if (!SoundSource)
	{
		F_LogError("Failed to play an instanced 2d sound.");
		return;
	}

	SoundSource->setDefaultVolume(Volume);
	irrklang::ISound* InstancedSound = SoundEngine->play2D(SoundSource);

	if (InstancedSound)
	{
		InstancedSound->drop();
	}
}

void FSound::Play3D(const Float32 Volume, const FVector3D& Position, const Float32 MinAttenDistance)
{
	F_Assert(IsValid(), "This class is in an invalid state.");
	irrklang::ISoundSource* SoundSource = GetSource();

	if (!SoundSource)
	{
		F_LogError("Failed to play a 3d sound.");
		return;
	}

	SoundSource->setDefaultVolume(Volume);

	using irrklang::vec3df;
	const vec3df InPosition = vec3df(Position.x, Position.y, Position.z);

	const bool PlayLooped = false;
	const bool StartPaused = false;
	const bool TrackSound = true;

	DropSound();
	Sound = SoundEngine->play3D(
		SoundSource,
		InPosition,
		PlayLooped,
		StartPaused, 
		TrackSound);

	F_LogErrorIf(!Sound, "TrackSound was specified but the returned pointer was null.");

	if (Sound)
	{
		Sound->setMinDistance(MinAttenDistance);
	}
}

void FSound::Play3DInstanced(const Float32 Volume, const FVector3D& Position, const Float32 MinAttenDistance)
{
	F_Assert(IsValid(), "This class is in an invalid state.");
	irrklang::ISoundSource* SoundSource = GetSource();

	if (!SoundSource)
	{
		F_LogError("Failed to play an instanced 3d sound.");
		return;
	}

	SoundSource->setDefaultVolume(Volume);

	const irrklang::vec3df InPosition(Position.x, Position.y, Position.z);

	const bool PlayLooped = false;
	const bool StartPaused = false;
	const bool TrackSound = true;

	irrklang::ISound* InstancedSound = SoundEngine->play3D(
		SoundSource, 
		InPosition, 
		PlayLooped, 
		StartPaused, 
		TrackSound);

	F_LogErrorIf(!InstancedSound, "TrackSound was specified but the returned pointer was null.");

	if (InstancedSound)
	{
		InstancedSound->setMinDistance(MinAttenDistance);
		InstancedSound->drop();
	}
}

void FSound::Stop()
{
	F_Assert(IsValid(), "This class is in an invalid state.");
	F_LogErrorIf(!Sound, "Stop was called without a managed Sound.");

	if (Sound)
	{
		Sound->stop();
	}
}

void FSound::StopAll()
{
	F_Assert(IsValid(), "This class is in an invalid state.");
	SoundEngine->removeSoundSource(File.c_str());
}

void FSound::SetPauseState(const bool IsSoundPaused)
{
	F_Assert(IsValid(), "This class is in an invalid state.");
	F_LogErrorIf(!Sound, "SetPauseState was called without a managed Sound.");

	if (Sound)
	{
		Sound->setIsPaused(IsSoundPaused);
	}
}

bool FSound::IsPaused() const
{
	F_Assert(IsValid(), "This class is in an invalid state.");

	if (!Sound)
	{
		F_LogError("IsPaused was called without a managed Sound.");
		return false;
	}

	const bool IsSoundPaused = Sound->getIsPaused();
	return IsSoundPaused;
}

bool FSound::IsFinished() const
{
	F_Assert(IsValid(), "This class is in an invalid state.");

	if (!Sound)
	{
		F_LogError("IsFinished was called without a managed Sound.");
		return false;
	}

	const bool IsSoundFinished = Sound->isFinished();
	return IsSoundFinished;
}

void FSound::DropSound()
{
	if (Sound)
	{
		Sound->stop();
		Sound->drop();
	}
}

irrklang::ISoundSource* FSound::GetSource()
{
	F_Assert(IsValid(), "This class is in an invalid state.");

	irrklang::ISoundSource* SoundSource = SoundEngine->getSoundSource(File.c_str());

	if (!SoundSource)
	{
		SoundSource = SoundEngine->addSoundSourceFromFile(File.c_str());
	}

	F_LogErrorIf(!SoundSource, "Failed to get/add SoundSource.")
	return SoundSource;
}
