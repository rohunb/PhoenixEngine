#include "Stdafx.h"
#include "Platform/Event/EventHandler.h"

using namespace Phoenix;

TVector<FEvent>& FEventHandler::GetEvents()
{
	return Events;
}

void FEventHandler::EventCallback(const FEvent& Event)
{
	AddEvent(Event);
}

void FEventHandler::AddEvent(const FEvent& Event)
{
	Events.push_back(Event);
}
