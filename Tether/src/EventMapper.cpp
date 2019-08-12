/*
 * EventMapper.cpp
 *
 *  Created on:	Sep 6, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "EventMapper.h"
#include <iostream>
#include "SFML/Window.hpp"

EventMapper::EventMapper(int statusSize):
stSize(statusSize),pm(1)
{
	status=new float[stSize];
	for(int i=0;i<stSize;i++)
	{
		status[i]=0;
	}
	std::cout<<"status[0]="<<status[0]<<" status[1]="<<status[1]<<std::endl;
	std::cout<<"status="<<status<<std::endl;
	std::cout<<"stSize="<<stSize<<std::endl;
}

EventMapper::~EventMapper()
{
	delete status;
}

float EventMapper::getStatus(int id)
{
	return status[id];
}

#include "WarnErrReporter.h"
EventMapper::EventMapper(std::string url):
pm(1)
{
	//TODO implement
	WarnErrReporter::notImplementedErr("EventMapper::EventMapper(std::string url) is not implemented yet. Despite this, this code was called: doing nothing...");
}

#include "ShortNames.h"
void EventMapper::event(EventHandler::event e)
{
	u32 size=ID.size();
	for (u32 i=0;i<size;i++)
	{
		if(ID[i]==e.ID)
		{
			mapping myM=m[i];
			bool evAsTrue=false;
			if(myM.condition==0)
				evAsTrue=true;
			else if(myM.condition>=stSize||myM.condition<=-stSize)
			{
				WarnErrReporter::invalidValueErr("Event handling condition points to outside of bounds index, ignoring this event...");
				continue;
			}
			if(myM.condition>0&&status[myM.condition]==0) evAsTrue=true;
			else if(myM.condition<0&&status[-myM.condition]!=0) evAsTrue=true;
			if(!evAsTrue) continue;
			if(myM.mode==MAPPER_MODE_EMPTY) (void)0;//do nothing
			else if(myM.mode==MAPPER_MODE_HOLD)
			{
				if(e.value>0)
					status[myM.statusIndex]+=myM.mapParam;//TODO multi-stage mapping
				else
					status[myM.statusIndex]-=myM.mapParam;
			}
			else if(myM.mode==MAPPER_MODE_TOGGLE)
			{
				if(e.value==myM.mapParam)
				{
					int indx=myM.statusIndex;
					std::cout<<"toggle "<<indx<<std::endl;
					if(status[indx]==0) status[indx]=1;//toggle between 0 and 1
					else status[indx]=0;
				}
			}
			else if(myM.mode==MAPPER_MODE_ADD)
			{
				status[myM.statusIndex]+=e.value+myM.mapParam;
			}
			else if(myM.mode==MAPPER_MODE_ADD_RESETM)
			{
				status[myM.statusIndex]+=e.value+myM.mapParam;
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
				status[myM.statusIndex]=e.value+myM.mapParam;
			}

			std::cout<<"ID[i]="<<ID[i]<<" status[myM.statusIndex]="<<status[myM.statusIndex]<<std::endl;
		}
	}
}

float EventMapper::getTimeRef()
{//TODO
	return 0;
}

float EventMapper::getStatusAndReset(int indx,float resetTo)
{
	float curStatus=status[indx];
	status[indx]=resetTo;
	return curStatus;
}

void EventMapper::registerAction(int id, int mode, int condition,int statusIndex, float mapParam)
{
		ID.push_back(id);
		m.push_back({mode,condition,statusIndex,mapParam});
}

void EventMapper::setConditionTrue(int id)//TODO use proper int kind as ID
{
	if(id<0||id>(int)ID.size())
	{
		WarnErrReporter::invalidValueErr("setConditionTrue was called with out of bounds ID value, ignoring this call...");
		return;
	}
	status[id]=1;
}

void EventMapper::setConditionFalse(int id)
{
	if(id<0||id>(int)ID.size())
	{
		WarnErrReporter::invalidValueErr("setConditionFalse was called with out of bounds ID value, ignoring this call...");
		return;
	}
	status[id]=0;
}

void EventMapper::setConditionORedTrue(int id)
{
	if(id<0||id>(int)ID.size())
	{
		WarnErrReporter::invalidValueErr("setConditionORedTrue was called with out of bounds ID value, ignoring this call...");
		return;
	}
	status[id]++;
}

void EventMapper::setConditionORedFalse(int id)
{
	if(id<0||id>(int)ID.size())
	{
		WarnErrReporter::invalidValueErr("setConditionORedFalse was called with out of bounds ID value, ignoring this call...");
		return;
	}
	status[id]--;
}

void EventMapper::toggleCondition(int id)
{
	if(id<0||id>(int)ID.size())
	{
		WarnErrReporter::invalidValueErr("toggleCondition was called with out of bounds ID value, ignoring this call...");
		return;
	}
	status[id]=!status[id];
}

void EventMapper::clearAllMappings()
{
	ID.clear();
	m.clear();
}
