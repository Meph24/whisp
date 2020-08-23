#include "EventMapper.h"
#include <iostream>
#include "SFML/Window.hpp"

EventMapper::EventMapper()
	: control_input_stati({})
	, pm(1)
{
	std::cout<<"control_input_stati[0]="<<control_input_stati[0]<<" control_input_stati[1]="<<control_input_stati[1]<<std::endl;
	std::cout<<"&control_input_stati"<< &control_input_stati <<std::endl;
	std::cout<<"control_input_stati.size="<<control_input_stati.size()<<std::endl;
}

float EventMapper::getStatus(int id)
{
	return control_input_stati[id];
}

#include "ShortNames.h"
void EventMapper::event(EventHandler::event e)
{
	for(mapping& myM : event_id_mappings[e.ID])
	{
		bool evAsTrue=false;
		if(myM.condition==0)
			evAsTrue=true;
		else if(myM.condition>=(int)control_input_stati.size()||myM.condition<=-(int)control_input_stati.size())
		{
			WarnErrReporter::invalidValueErr("Event handling condition points to outside of bounds index, ignoring this event...");
			continue;
		}
		if(myM.condition>0&&control_input_stati[myM.condition]==0) evAsTrue=true;
		else if(myM.condition<0&&control_input_stati[-myM.condition]!=0) evAsTrue=true;
		if(!evAsTrue) continue;
		if(myM.mode==MAPPER_MODE_EMPTY) (void)0;//do nothing
		else if(myM.mode==MAPPER_MODE_HOLD)
		{
			if(e.value>0)
				control_input_stati[myM.statusIndex]+=myM.mapParam;//TODO multi-stage mapping
			else
				control_input_stati[myM.statusIndex]-=myM.mapParam;
		}
		else if(myM.mode==MAPPER_MODE_TOGGLE)
		{
			if(e.value==myM.mapParam)
			{
				int indx=myM.statusIndex;
				std::cout<<"toggle "<<indx<<std::endl;
				if(control_input_stati[indx]==0) control_input_stati[indx]=1;//toggle between 0 and 1
				else control_input_stati[indx]=0;
			}
		}
		else if(myM.mode==MAPPER_MODE_ADD)
		{
			control_input_stati[myM.statusIndex]+=e.value+myM.mapParam;
		}
		else if(myM.mode==MAPPER_MODE_ADD_RESETM)
		{
			control_input_stati[myM.statusIndex]+=e.value+myM.mapParam;
			if(e.ID==2048)//mouse x
			{
				sf::Vector2i mousePos=sf::Mouse::getPosition();
				mousePos.x=-myM.mapParam;
				sf::Mouse::setPosition(mousePos);
			}
			else if(e.ID==2049)//mouse y
			{
				sf::Vector2i mousePos=sf::Mouse::getPosition();
				mousePos.y=-myM.mapParam;
				sf::Mouse::setPosition(mousePos);
			}
		}
		else if(myM.mode==MAPPER_MODE_REPLACE)
		{
			control_input_stati[myM.statusIndex]=e.value+myM.mapParam;
		}

		std::cout << "InputEvent::id" << e.ID << " control_input_stati[myM.statusIndex]=" << control_input_stati[myM.statusIndex] << std::endl;
	}
}

float EventMapper::getStatusAndReset(int indx,float resetTo)
{
	float ret = control_input_stati[indx];
	control_input_stati[indx]=resetTo;
	return ret;
}

void EventMapper::registerAction(int id, int mode, int condition,int statusIndex, float mapParam)
{
		event_id_mappings[id].push_back({mode,condition,statusIndex,mapParam});
}

void EventMapper::setConditionTrue(int id)//TODO use proper int kind as ID
{
	if(id<0||id>(int)control_input_stati.size())
	{
		WarnErrReporter::invalidValueErr("setConditionTrue was called with out of bounds ID value, ignoring this call...");
		return;
	}
	control_input_stati[id]=1;
}

void EventMapper::setConditionFalse(int id)
{
	if(id<0||id>(int)control_input_stati.size())
	{
		WarnErrReporter::invalidValueErr("setConditionFalse was called with out of bounds ID value, ignoring this call...");
		return;
	}
	control_input_stati[id]=0;
}

void EventMapper::setConditionORedTrue(int id)
{
	if(id<0||id>(int)control_input_stati.size())
	{
		WarnErrReporter::invalidValueErr("setConditionORedTrue was called with out of bounds ID value, ignoring this call...");
		return;
	}
	control_input_stati[id]++;
}

void EventMapper::setConditionORedFalse(int id)
{
	if(id<0||id>(int)control_input_stati.size())
	{
		WarnErrReporter::invalidValueErr("setConditionORedFalse was called with out of bounds ID value, ignoring this call...");
		return;
	}
	control_input_stati[id]--;
}

void EventMapper::toggleCondition(int id)
{
	if(id<0||id>(int)control_input_stati.size())
	{
		WarnErrReporter::invalidValueErr("toggleCondition was called with out of bounds ID value, ignoring this call...");
		return;
	}
	control_input_stati[id]=!control_input_stati[id];
}

void EventMapper::clearAllMappings(){ event_id_mappings.clear(); }
