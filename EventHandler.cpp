#include "EventHandler.h"
#include <iostream>

EventHandler::Filter::Filter()
{
	prevValue = new float[NUM_EVENTTYPES << EVENTHANDLER_LOWID_BITS];
	for (int i = 0; i < (NUM_EVENTTYPES << EVENTHANDLER_LOWID_BITS); i++)
	{
		prevValue[i] = 0;
	}
}
EventHandler::Filter::~Filter()
{
	delete[] prevValue;
}

bool EventHandler::Filter::filter(EventHandler::event e)
{
	if (e.ID == 4) return true;
	if (prevValue[e.ID] == e.value) return false;
	return true;
}

void EventHandler::Filter::updateFilter(event e)
{
	prevValue[e.ID] = e.value;
}

void EventHandler::sendOn(EventHandler::event e)
{
	std::cout << e.ID << "/" << e.value << std::endl;
	//handon for higher shit
}

void EventHandler::handle(EventHandler::event e)
{
	if (Filter.filter(e))
	{
		Filter.updateFilter(e);
		sendOn(e);
	}
}

EventHandler::event EventHandler::createEvent(EventHandler::eventType type, int lowID, float value)
{
	EventHandler::event newEvent;
	newEvent.ID = (type << EVENTHANDLER_LOWID_BITS) | lowID;
	newEvent.value = value;
	return newEvent;
}