#ifndef PHOENIX_AUDIO_LISTENER_H
#define PHOENIX_AUDIO_LISTENER_H

#include "ExternalLib/IrrKlangIncludes.h"
#include "Math/Vector3D.h"

namespace Phoenix
{
	class FAudioListener
	{
	public:
		static const FVector3D DefaultPosition;
		static const FVector3D DefaultLookDirection;
		static const FVector3D DefaultVelocityPerSec;
		static const FVector3D DefaultUpVector;

		FAudioListener() = default;

		FAudioListener(const FAudioListener&) = delete;
		FAudioListener& operator=(const FAudioListener&) = delete;

		FAudioListener(FAudioListener&&) = delete;
		FAudioListener& operator=(FAudioListener&&) = delete;

		~FAudioListener();

		void Init(irrklang::ISoundEngine* InSoundEngine);

		bool IsValid() const;

		void DeInit();

		void Set(
			const FVector3D& InPosition,
			const FVector3D& InLookDirection,
			const FVector3D& InVelocityPerSec,
			const FVector3D& InUpVector);

		void SetPosition(const FVector3D& InPosition);

		void SetLookDirection(const FVector3D& InLookDirection);

		void SetVelocityPerSec(const FVector3D& InVelocityPerSec);

		void SetUpVector(const FVector3D& InUpVector);

		const FVector3D& GetPosition() const;

		const FVector3D& GetLookDirection() const;

		const FVector3D& GetVelocityPerSec() const;

		const FVector3D& GetUpVector() const;

	protected:
	private:
		irrklang::ISoundEngine* SoundEngine{ nullptr };
		FVector3D Position{ FAudioListener::DefaultPosition };
		FVector3D LookDirection{ FAudioListener::DefaultLookDirection };
		FVector3D VelocityPerSec{ FAudioListener::DefaultVelocityPerSec };
		FVector3D UpVector{ FAudioListener::DefaultUpVector };

		void UpdateSoundEngineValues() const;
	};
}

#endif