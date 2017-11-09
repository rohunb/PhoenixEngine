#ifndef PHOENIX_SOUND_H
#define PHOENIX_SOUND_H

#include "ExternalLib/IrrKlangIncludes.h"
#include "Utility/Misc/String.h"
#include "Math/Vector3D.h"

namespace Phoenix
{
	class FSound
	{
	public:
		static const Float32 MinAttenDistance;

		FSound() = default;

		FSound(const FSound&) = delete;
		FSound& operator=(const FSound&) = delete;

		FSound(FSound&&) = delete;
		FSound& operator=(FSound&&) = delete;

		~FSound();

		void Init(irrklang::ISoundEngine* InSoundEngine, const FString& InFile);

		bool IsValid() const;

		void DeInit();

		void SetFile(const FString& InFile);

		void SetVolume(const Float32 Volume);

		const FString& GetFile() const;

		Float32 GetVolume() const;
		
		void Play2D(const Float32 Volume, const bool LoopSound);

		void Play2DInstanced(const Float32 Volume);

		void Play3D(const Float32 Volume, const FVector3D& Position, const Float32 MinAttenDistance);

		void Play3DInstanced(const Float32 Volume, const FVector3D& Position, const Float32 MinAttenDistance);

		void Stop();

		void StopAll();

		void SetPauseState(const bool IsSoundPaused);

		bool IsPaused() const;

		bool IsFinished() const;
		
	protected:
	private:
		irrklang::ISoundEngine* SoundEngine{ nullptr };
		irrklang::ISound* Sound{ nullptr };
		FString File;

		void DropSound();

		irrklang::ISoundSource* GetSource();
	};
}

#endif