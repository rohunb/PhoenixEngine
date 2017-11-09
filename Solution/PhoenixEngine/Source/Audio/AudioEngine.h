#ifndef PHOENIX_AUDIO_ENGINE_H
#define PHOENIX_AUDIO_ENGINE_H

#include "ExternalLib/IrrKlangIncludes.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/String.h"
#include "Math/Vector3D.h"
#include "Audio/AudioListener.h"
#include "Audio/Sound.h"

namespace Phoenix
{
	class FAudioEngine {
	public:
		static const Float32 MinVolume;
		static const Float32 MaxVolume;

		FAudioEngine() = default;

		FAudioEngine(const FAudioEngine&) = delete;
		FAudioEngine& operator=(const FAudioEngine&) = delete;

		FAudioEngine(FAudioEngine&&) = delete;
		FAudioEngine& operator=(FAudioEngine&&) = delete;

		~FAudioEngine();

		void Init();

		bool IsValid() const;

		void DeInit();
		
		void SetMasterVolume(const Float32 Volume);

		Float32 GetMasterVolume() const;

		FAudioListener& GetAudioListener();

		const FAudioListener& GetAudioListener() const;

		void Play2D(const FString& File, const Float32 Volume);

		void Play2D(const FChar* const File, const Float32 Volume);

		void Play3D(const FString& File, const Float32 Volume, const FVector3D& Position);

		void Play3D(const FChar* const File, const Float32 Volume, const FVector3D& Position);

		void PauseAllSounds();

		void UnpauseAllSounds();

		void StopAllSounds();

	protected:
	private:
		irrklang::ISoundEngine* SoundEngine { nullptr };
		FAudioListener AudioListener;
	};
}

#endif