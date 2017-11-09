#include "Stdafx.h"
#include "Audio/AudioEngine.h"

#include "Utility/Debug/Assert.h"
#include "Utility/Debug/Debug.h"
#include "Utility/Debug/Profiler.h"

using namespace Phoenix;

const Float32 FAudioEngine::MinVolume = 0.f;
const Float32 FAudioEngine::MaxVolume = 1.f;

FAudioEngine::~FAudioEngine()
{
	DeInit();
}

void FAudioEngine::Init()
{
	DeInit();
	F_Profile();
	SoundEngine = irrklang::createIrrKlangDevice();

	if (!SoundEngine)
	{
		F_LogError("Failed to create IrrKlang device.");
		DeInit();
		return;
	}

	AudioListener.Init(SoundEngine);
}

bool FAudioEngine::IsValid() const
{
	const bool IsSoundEngineValid = SoundEngine != nullptr;
	const bool IsAudioListenerValid = AudioListener.IsValid();

	const bool Result =
		IsSoundEngineValid &&
		IsAudioListenerValid;

	return Result;
}

void FAudioEngine::DeInit()
{
	AudioListener.DeInit();

	if (!SoundEngine)
	{
		return;
	}

	if (!SoundEngine->drop())
	{
		F_LogError("AudioEngine was shut down but an asset still has a strong reference.");
	}

	SoundEngine = nullptr;
}

void FAudioEngine::SetMasterVolume(const Float32 Volume)
{
	F_Assert(IsValid(), "This class must be valid.");
	F_Assert(Volume >= FAudioEngine::MinVolume, "Volume is invalid.");
	F_Assert(Volume <= FAudioEngine::MaxVolume, "Volume is invalid.");

	SoundEngine->setSoundVolume(Volume);
}

Float32 FAudioEngine::GetMasterVolume() const
{
	F_Assert(IsValid(), "This class must be valid.");
	const Float32 MasterVolume = SoundEngine->getSoundVolume();
	return MasterVolume;
}

FAudioListener& FAudioEngine::GetAudioListener()
{
	return AudioListener;
}

const FAudioListener& FAudioEngine::GetAudioListener() const
{
	return AudioListener;
}

void FAudioEngine::Play2D(const FString& File, const Float32 Volume)
{
	F_Assert(IsValid(), "This class must be valid.");
	Play2D(File.c_str(), Volume);
}

void FAudioEngine::Play2D(const FChar* const File, const Float32 Volume)
{
	F_Assert(IsValid(), "This class must be valid.");
	F_Assert(File, "File should not be null.");
	irrklang::ISoundSource* SoundSource = SoundEngine->getSoundSource(File);

	if (!SoundSource)
	{
		SoundSource = SoundEngine->addSoundSourceFromFile(File);
	}

	if (!SoundSource)
	{
		F_LogError("Failed to get/add sound source for file: {" << File << "}");
		return;
	}

	SoundSource->setDefaultVolume(Volume);
	irrklang::ISound* Sound2D = SoundEngine->play2D(SoundSource);

	if (Sound2D)
	{
		Sound2D->drop();
	}
}

void FAudioEngine::Play3D(const FString& File, const Float32 Volume, const FVector3D& Position)
{
	F_Assert(IsValid(), "This class must be valid.");
	Play3D(File.c_str(), Volume, Position);
}

void FAudioEngine::Play3D(const FChar* const File, const Float32 Volume, const FVector3D& Position)
{
	F_Assert(IsValid(), "This class must be valid.");
	F_Assert(File, "File should not be null.");
	irrklang::ISoundSource* SoundSource = SoundEngine->getSoundSource(File);

	if (!SoundSource)
	{
		SoundSource = SoundEngine->addSoundSourceFromFile(File);
	}

	if (!SoundSource)
	{
		F_LogError("Failed to get/add sound source for file: {" << File << "}");
		return;
	}

	SoundSource->setDefaultVolume(Volume);
	const irrklang::vec3df IrrPosition(Position.x, Position.y, Position.z);
	irrklang::ISound* Sound3D = SoundEngine->play3D(SoundSource, IrrPosition);

	if (Sound3D)
	{
		Sound3D->drop();
	}
}

void FAudioEngine::PauseAllSounds()
{
	F_Assert(IsValid(), "This class must be valid.");
	SoundEngine->setAllSoundsPaused(true);
}

void FAudioEngine::UnpauseAllSounds()
{
	F_Assert(IsValid(), "This class must be valid.");
	SoundEngine->setAllSoundsPaused(false);
}

void FAudioEngine::StopAllSounds()
{
	F_Assert(IsValid(), "This class must be valid.");
	SoundEngine->stopAllSounds();
}
