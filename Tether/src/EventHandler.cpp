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
extern MathModel * myModel;
extern RobonovaGraphics * robG;
static int selected = 0;
static int on = 0;
#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"
#include "Zombie_World.h"
extern Zombie_KeyInput * keyInput;
extern Zombie_MouseInput * mouseInput;
extern int enabledProgram;
extern Zombie_World * world;
#include "POCmain.h"
extern POCmain * poc;

#include "CameraTP.h"
void EventHandler::sendOn(EventHandler::event e)
{
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
				world->cam->zoom/=zoomMult;
				world->mouseInp->sensitivityX/=zoomMult;
				world->mouseInp->sensitivityY/=zoomMult;
			}
			else
			{
				world->cam->zoom*=zoomMult;
				world->mouseInp->sensitivityX*=zoomMult;
				world->mouseInp->sensitivityY*=zoomMult;
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
	else if(enabledProgram==0)//robot
	{

		int change = 0;
		if (e.ID == 2048)//mouse x
		{
			if (on) robG->cam->beta = e.value / 2;//myModel->left.a1 = e.value/10;
		}
		else if (e.ID == 2049)//mouse y
		{
			if (on) robG->cam->alpha = e.value / 2;
			//myModel->left.a1 = e.value/10;
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
				myModel->left.a0 += mov;
				//if (myModel->left.a0 < -2) myModel->left.a0 = -2;
				break;
			case 1:
				myModel->left.a1 += mov;
				break;
			case 2:
				myModel->left.a2 += mov;
				break;
			case 3:
				myModel->left.a3 += mov;
				break;
			case 4:
				myModel->left.a4 += mov;
				break;
			case 5:
				myModel->right.a0 += mov;
				break;
			case 6:
				myModel->right.a1 += mov;
				break;
			case 7:
				myModel->right.a2 += mov;
				break;
			case 8:
				myModel->right.a3 += mov;
				break;
			case 9:
				myModel->right.a4 += mov;
				break;
			case 10:
				myModel->leftArm.a0 += mov;
				break;
			case 11:
				myModel->leftArm.a1 += mov;
				break;
			case 12:
				myModel->leftArm.a2 += mov;
				break;
			case 13:
				myModel->rightArm.a0 += mov;
				break;
			case 14:
				myModel->rightArm.a1 += mov;
				break;
			case 15:
				myModel->rightArm.a2 += mov;
				break;
			}
		}
	}
	else if(enabledProgram==2)
	{
		poc->event(e);
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
