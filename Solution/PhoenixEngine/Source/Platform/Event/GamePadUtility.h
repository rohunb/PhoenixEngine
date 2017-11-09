#ifndef PHOENIX_GAME_PAD_UTILITY_H
#define PHOENIX_GAME_PAD_UTILITY_H

#include "Utility/Containers/Array.h"
#include "Utility/Containers/BitArray.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Misc/Memory.h"
#include "Utility/Misc/Primitives.h"
#include "Platform/Input/GamePadEnums.h"

namespace Phoenix
{
	typedef Float32 FGamePadAxisT;
	typedef UInt8 FGamePadButtonT;

	class FGamePadState
	{
	public:
		FGamePadState() = default;

		FGamePadState(const FGamePadState&) = delete;
		FGamePadState& operator=(const FGamePadState&) = delete;

		FGamePadState(FGamePadState&&) = delete;
		FGamePadState& operator=(FGamePadState&&) = delete;

		~FGamePadState();

		void Init(const EGamePadID::Value InGamePadID, class FEventHandler& EventHandler);

		bool IsValid() const;

		void DeInit(class FEventHandler& EventHandler);

		void ProcessEvents(class FEventHandler& EventHandler);

		const FChar* GetName() const;

	private:
		static const Float32 AxisPickUpRange;

		EGamePadID::Value GamePadID{ EGamePadID::Unknown };
		TVector<FGamePadAxisT> Axes;
		TVector<FGamePadButtonT> Buttons;

		void InitAxes(class FEventHandler& EventHandler);

		void InitButtons(class FEventHandler& EventHandler);

		void ResetAxes(class FEventHandler& EventHandler);

		void ResetButtons(class FEventHandler& EventHandler);

		void ProcessAxisEvents(class FEventHandler& EventHandler);

		void ProcessButtonEvents(class FEventHandler& EventHandler);
	};

	class FGamePadUtility
	{
	public:
		FGamePadUtility();

		FGamePadUtility(const FGamePadUtility&) = delete;
		FGamePadUtility& operator=(const FGamePadUtility&) = delete;
		
		FGamePadUtility(FGamePadUtility&&) = delete;
		FGamePadUtility& operator=(FGamePadUtility&&) = delete;

		~FGamePadUtility();

		void Init(class FEventHandler& EventHandler);

		bool IsValid() const;

		void DeInit(class FEventHandler& EventHandler);

		void ProcessEvents(class FEventHandler& EventHandler);

	private:
		static const EGamePadID::Type MaxGamePadsConfig;

		TBitArray<EGamePadID::Count> ActiveGamePads;
		TArray<FGamePadState, EGamePadID::Count> GamePads;

		void SetGamePadState(const EGamePadID::Value GamePadID, const bool IsActive);

		bool GamePadIsActive(const EGamePadID::Value GamePadID) const;
	};
}

#endif
