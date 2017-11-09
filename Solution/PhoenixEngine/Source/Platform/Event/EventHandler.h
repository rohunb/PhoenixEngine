#ifndef PHOENIX_EVENT_HANDLER_H
#define PHOENIX_EVENT_HANDLER_H

#include "Utility/Containers/Vector.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/StaticObject.h"
#include "Platform/Event/Event.h"
#include "Platform/Event/EventTypes.h"
#include "Platform/Input/InputAction.h"
#include "Platform/Input/KeyMods.h"
#include "Platform/Input/Keys.h"
#include "Platform/Input/MouseButton.h"

namespace Phoenix
{
	class FEventHandler
	{
		F_AddStaticObjectToClass(FEventHandler);
	public:
		FEventHandler() = default;

		FEventHandler(const FEventHandler&) = delete;
		FEventHandler& operator=(const FEventHandler&) = delete;

		FEventHandler(FEventHandler&&) = delete;
		FEventHandler& operator=(FEventHandler&&) = delete;

		TVector<FEvent>& GetEvents();

		void EventCallback(const FEvent& Event);

		template <class... Arguments>
		void GamePadEventCallback(Arguments&&... Args);

		template <class... Arguments>
		void KeyEventCallback(Arguments&&... Args);

		template<class... Arguments>
		void MovementAxisCallback(Arguments&&... Args);

		template <class... Arguments>
		void MouseEventCallback(Arguments&&... Args);

		template <class... Arguments>
		void WindowEventCallback(Arguments&&... Args);

	protected:
	private:
		TVector<FEvent> Events;

		void AddEvent(const FEvent& Event);
	};

	template <class... Arguments>
	void FEventHandler::GamePadEventCallback(Arguments&&... Args)
	{
		FEvent Event;
		Event.GamePad = FGamePadEvent(std::forward<Arguments>(Args)...);

		AddEvent(Event);
	}

	template <class... Arguments>
	void FEventHandler::KeyEventCallback(Arguments&&... Args)
	{
		FEvent Event;
		Event.Key = FKeyEvent(std::forward<Arguments>(Args)...);

		AddEvent(Event);
	}

	template<class... Arguments>
	void FEventHandler::MovementAxisCallback(Arguments&&... Args)
	{
		FEvent Event;
		Event.MovementAxis = FMovementAxisEvent(std::forward<Arguments>(Args)...);

		AddEvent(Event);
	}

	template <class... Arguments>
	void FEventHandler::MouseEventCallback(Arguments&&... Args)
	{
		FEvent Event;
		Event.Mouse = FMouseEvent(std::forward<Arguments>(Args)...);

		AddEvent(Event);
	}

	template <class... Arguments>
	void FEventHandler::WindowEventCallback(Arguments&&... Args)
	{
		FEvent Event;
		Event.Window = FWindowEvent(std::forward<Arguments>(Args)...);

		AddEvent(Event);
	}
}

#endif
