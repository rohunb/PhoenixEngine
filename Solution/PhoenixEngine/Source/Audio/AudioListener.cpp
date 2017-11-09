#include "Stdafx.h"
#include "AudioListener.h"

#include "Utility/Debug/Assert.h"
#include "Math/Math.h"

using namespace Phoenix;

const FVector3D FAudioListener::DefaultPosition = FVector3D(0, 0, 0);
const FVector3D FAudioListener::DefaultLookDirection = FVector3D(0, 0, -1);
const FVector3D FAudioListener::DefaultVelocityPerSec = FVector3D(0, 0, 0);
const FVector3D FAudioListener::DefaultUpVector = FVector3D(0, 1, 0);

FAudioListener::~FAudioListener()
{
	DeInit();
}

void FAudioListener::Init(irrklang::ISoundEngine* InSoundEngine)
{
	F_Assert(InSoundEngine, "SoundEngine must not be null.");
	DeInit();

	SoundEngine = InSoundEngine;
	SoundEngine->grab();
}

bool FAudioListener::IsValid() const
{
	const bool IsSoundEngineValid = SoundEngine != nullptr;
	return IsSoundEngineValid;
}

void FAudioListener::DeInit()
{
	if (!SoundEngine)
	{
		return;
	}

	SoundEngine->drop();
	SoundEngine = nullptr;
}

void FAudioListener::Set(
	const FVector3D& InPosition,
	const FVector3D& InLookDirection,
	const FVector3D& InVelocityPerSec,
	const FVector3D& InUpVector)
{
	F_Assert(FMathf::IsCloseTo(glm::length(InLookDirection), 1.f),
		"LookDirection is not normalized.  Current length: " 
		<< std::fixed << glm::length(InLookDirection));
	F_Assert(FMathf::IsCloseTo(glm::length(InUpVector), 1.f),
		"UpVector is not normalized.  Current length: "
		<< std::fixed << glm::length(InUpVector));

	Position = InPosition;
	LookDirection = InLookDirection;
	VelocityPerSec = InVelocityPerSec;
	UpVector = InUpVector;
	UpdateSoundEngineValues();
}

void FAudioListener::SetPosition(const FVector3D& InPosition)
{
	Position = InPosition;
	UpdateSoundEngineValues();
}

void FAudioListener::SetLookDirection(const FVector3D& InLookDirection)
{
	F_Assert(FMathf::IsCloseTo(glm::length(InLookDirection), 1.f),
		"UpVector is not normalized.  Current length: "
		<< std::fixed << glm::length(InLookDirection));

	LookDirection = InLookDirection;
	UpdateSoundEngineValues();
}

void FAudioListener::SetVelocityPerSec(const FVector3D& InVelocityPerSec)
{
	VelocityPerSec = InVelocityPerSec;
	UpdateSoundEngineValues();
}

void FAudioListener::SetUpVector(const FVector3D& InUpVector)
{
	F_Assert(FMathf::IsCloseTo(glm::length(InUpVector), 1.f),
		"UpVector is not normalized.  Current length: " 
		<< std::fixed << glm::length(InUpVector));

	UpVector = InUpVector;
	UpdateSoundEngineValues();
}

const FVector3D& FAudioListener::GetPosition() const
{
	return Position;
}

const FVector3D& FAudioListener::GetLookDirection() const
{
	return LookDirection;
}

const FVector3D& FAudioListener::GetVelocityPerSec() const
{
	return VelocityPerSec;
}

const FVector3D& FAudioListener::GetUpVector() const
{
	return UpVector;
}

void FAudioListener::UpdateSoundEngineValues() const
{
	F_Assert(IsValid(), "This class must be valid.");

	const irrklang::vec3df InPosition(Position.x, Position.y, Position.z);
	const irrklang::vec3df InLookDirection(LookDirection.x, LookDirection.y, LookDirection.z);
	const irrklang::vec3df InVelocityPerSec(VelocityPerSec.x, VelocityPerSec.y, VelocityPerSec.z);
	const irrklang::vec3df InUpVector(UpVector.x, UpVector.y, UpVector.z);

	SoundEngine->setListenerPosition(InPosition, InLookDirection, InVelocityPerSec, InUpVector);
}
