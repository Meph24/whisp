
#include "EventHandler.h"
#include "EntityPlayer.h"
#include "EventMapper.hpp"


#include <iostream>

EventHandler::EventHandler(IGameMode* sim, EventMapper* event_mapper) : sim(sim), event_mapper(event_mapper) {}

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
	switch (e.ID)
	{
	case 4:
		//text entered events
		return true;			
		break;

		//Mousewheel 1 events have to be excluded due to no reset event(value = 0) will be sent
	case 2050:
		return true;
		break;

		//Mousewheel 1 events have to be excluded due to no reset event(value = 0) will be sent
	case 2051:
		return true;
		break;
	default:
		if (prevValue[e.ID] == e.value) return false;
		return true;
		break;
	}
}

void EventHandler::Filter::updateFilter(event e)
{
	prevValue[e.ID] = e.value;
}

#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"

#include "CameraTP.h"


void EventHandler::sendOn(EventHandler::event e)
{
	if(event_mapper) event_mapper->event(e);
	Zombie_MouseInput * mouseInp=0;
	if(sim) if(sim->player) mouseInp=sim->player->mouseInp;
	Zombie_KeyInput * keyInput=0;
	if(sim) if(sim->player) keyInput=sim->player->keyInp;
	if (e.ID == 2048)
	{
		if (mouseInp)
			mouseInp->mouseMovedX(e.value);
	}
	else if (e.ID == 2049)
	{
		if (mouseInp)
			mouseInp->mouseMovedY(e.value);
	}
	else if (e.ID == 1024 + ('W' - 'A'))
	{
		if (keyInput)
			keyInput->walkForward(e.value);
	}
	else if (e.ID == 1024 + ('S' - 'A'))
	{
		if (keyInput)
			keyInput->walkBack(e.value);
	}
	else if (e.ID == 1024 + ('A' - 'A'))
	{
		if (keyInput)
			keyInput->walkLeft(e.value);
	}
	else if (e.ID == 1024 + ('D' - 'A'))
	{
		if (keyInput)
			keyInput->walkRight(e.value);
	}
	else if ((e.ID == 1060) && e.value)
	{
		if (keyInput)
			keyInput->menuButton();
	}
	else if (e.ID == 2052)
	{
		if(sim) sim->trigger(e.value);
	}
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
