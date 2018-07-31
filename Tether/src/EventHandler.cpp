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
#include "Zombie_World.h"
extern Zombie_KeyInput * keyInput;
extern Zombie_MouseInput * mouseInput;
extern int enabledProgram;
extern Zombie_World * world;

#include "CameraTP.h"
void EventHandler::sendOn(EventHandler::event e)
{
	//TODO dont hardcode stuff
	if ((e.ID == 1039)&&e.value)//p
	{
		enabledProgram++;
		std::cout << e.ID << "/" << e.value << std::endl;
		return;
	}
	std::cout << e.ID << "/" << e.value << std::endl;
	if (enabledProgram==1)//zombie
	{
		if (e.ID == 2048)
		{
			if (mouseInput)
				mouseInput->mouseMovedX(e.value);
		}
		else if (e.ID == 2049)
		{
			if (mouseInput)
				mouseInput->mouseMovedY(e.value);
		}
		else if ((e.ID == 1111)&&e.value)
		{
			world->debugScreen=!world->debugScreen;
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
			world->trigger(e.value);
		}
		else if (e.ID == 2050)
		{
			world->switchWeapon(e.value);
		}
		else if (e.ID == 2053)
		{
			float zoomMult=8;
			if(e.value)
			{
				world->player->cam->zoom/=zoomMult;
				world->player->mouseInp->sensitivityX/=zoomMult;//TODO find better way
				world->player->mouseInp->sensitivityY/=zoomMult;
			}
			else
			{
				world->player->cam->zoom*=zoomMult;
				world->player->mouseInp->sensitivityX*=zoomMult;
				world->player->mouseInp->sensitivityY*=zoomMult;
			}
		}
		else if ((e.ID == 1049) && e.value)
		{
			if (world->tm.targetRate == 1.0f)
				world->tm.targetRate = 0.1f;
			else world->tm.targetRate = 1.0f;
		}
		else if ((e.ID == 1024 + ('R' - 'A')) && e.value)
		{
			world->markRestart();
		}
		
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
