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

//TEST
#include "MathModel.h"
#include "RobonovaGraphics.h"
extern MathModel * model;
extern RobonovaGraphics * robG;
static int selected = 0;
static int on = 0;
#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"
#include "Zombie_World.h"
extern Zombie_KeyInput * keyInput;
extern Zombie_MouseInput * mouseInput;
extern bool zombieEnabled;
extern Zombie_World * world;

#include "CameraTP.h"
void EventHandler::sendOn(EventHandler::event e)
{
	if ((e.ID == 1039)&&e.value)//p
	{
		zombieEnabled = !zombieEnabled;
		std::cout << e.ID << "/" << e.value << std::endl;
		return;
	}
	std::cout << e.ID << "/" << e.value << std::endl;
	if (zombieEnabled)
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
			if(e.value)
			{
				world->cam->zoom/=8;
				world->mouseInp->sensitivityX/=8;
				world->mouseInp->sensitivityY/=8;
			}
			else
			{
				world->cam->zoom*=8;
				world->mouseInp->sensitivityX*=8;
				world->mouseInp->sensitivityY*=8;
			}
		}
		else if ((e.ID == 1049) && e.value)
		{
			if (world->timeFactor == 1.0f)
				world->timeFactor = 0.1f;
			else world->timeFactor = 1.0f;
		}
		else if ((e.ID == 1024 + ('R' - 'A')) && e.value)
		{
			world->markRestart();
		}
		
	}
	else
	{

		int change = 0;
		if (e.ID == 2048)//mouse x
		{
			if (on) robG->cam->beta = e.value / 2;//model->left.a1 = e.value/10;
		}
		else if (e.ID == 2049)//mouse y
		{
			if (on) robG->cam->alpha = e.value / 2;
			//model->left.a1 = e.value/10;
		}
		else if ((e.ID >= 1050) && (e.ID <= 1059))//0-9
		{
			selected = e.ID - 1050;
		}
		else if ((e.ID >= 1024) && (e.ID <= 1031))//A-F
		{
			selected = e.ID - 1014;
		}
		else if ((e.ID == 1091) && (e.value == 1))//+
		{
			change = 5;
		}
		else if ((e.ID == 1092) && (e.value == 1))//-
		{
			change = -5;
		}
		else if ((e.ID == 1081) && (e.value == 1))//SPACE
		{
			if (robG->rotEn)
			{
				robG->rotEn = 0;
				on = 1;
			}
			else
			{
				robG->rotEn = 1;
				on = 0;
			}
		}

		if (change != 0)
		{
			int mov = change;
			switch (selected)
			{
			case 0:
				model->left.a0 += mov;
				//if (model->left.a0 < -2) model->left.a0 = -2;
				break;
			case 1:
				model->left.a1 += mov;
				break;
			case 2:
				model->left.a2 += mov;
				break;
			case 3:
				model->left.a3 += mov;
				break;
			case 4:
				model->left.a4 += mov;
				break;
			case 5:
				model->right.a0 += mov;
				break;
			case 6:
				model->right.a1 += mov;
				break;
			case 7:
				model->right.a2 += mov;
				break;
			case 8:
				model->right.a3 += mov;
				break;
			case 9:
				model->right.a4 += mov;
				break;
			case 10:
				model->leftArm.a0 += mov;
				break;
			case 11:
				model->leftArm.a1 += mov;
				break;
			case 12:
				model->leftArm.a2 += mov;
				break;
			case 13:
				model->rightArm.a0 += mov;
				break;
			case 14:
				model->rightArm.a1 += mov;
				break;
			case 15:
				model->rightArm.a2 += mov;
				break;
			}
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
