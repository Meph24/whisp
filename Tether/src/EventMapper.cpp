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

EventMapper::EventMapper(int arraySize,int statusSize):
stSize(statusSize),arrSize(arraySize),pm(1)
{
	status=new float[stSize];
	for(int i=0;i<stSize;i++)
	{
		status[i]=0;
	}
	std::cout<<"status[0]="<<status[0]<<" status[1]="<<status[1]<<std::endl;
	ID=new int[arrSize];
	m=new mapping[arrSize];
	for(int i=0;i<arrSize;i++)
	{
		ID[i]=-1;
	}
	std::cout<<"status="<<status<<std::endl;
	std::cout<<"ID="<<ID<<std::endl;
	std::cout<<"m="<<m<<std::endl;
	std::cout<<"arrSize="<<arrSize<<std::endl;
	std::cout<<"stSize="<<stSize<<std::endl;
}

EventMapper::~EventMapper()
{
	delete m;
	delete ID;
	delete status;
}

float EventMapper::getStatus(int ID)
{
	return status[ID];
}

EventMapper::EventMapper(std::string url):
pm(1)
{//TODO implement
	std::cout<<"NOT IMPLEMENTED YET"<<std::endl;
}

void EventMapper::event(EventHandler::event e)
{
	for (int i=0;i<arrSize;i++)
	{
		if(ID[i]==e.ID)
		{
			mapping myM=m[i];
			bool evAsTrue=false;
			if(myM.condition==0)
				evAsTrue=true;
			else if(myM.condition>=arrSize||myM.condition<=-arrSize)
				continue;//TODO error
			if(myM.condition>0&&status[myM.condition]==0) evAsTrue=true;
			else if(myM.condition<0&&status[-myM.condition]!=0) evAsTrue=true;
			if(!evAsTrue) continue;
			if(myM.mode==MAPPER_MODE_EMPTY) ;//do nothing
			else if(myM.mode==MAPPER_MODE_HOLD)
			{
				if(e.value>0)
					status[myM.statusIndex]+=myM.mapParam;//TODO multi-stage mapping
				else
					status[myM.statusIndex]-=myM.mapParam;
			}
			else if(myM.mode==MAPPER_MODE_TOGGLE)
			{
				if(e.ID==myM.mapParam)
				{
					int indx=myM.statusIndex;
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
		}
	}
}

float EventMapper::getTimeRef()
{//TODO
}

float EventMapper::getStatusAndReset(int indx,float resetTo)
{
	//TODO implement thread safe option
	float curStatus=status[indx];
	status[indx]=resetTo;
	return curStatus;
}

void EventMapper::registerAction(int id, int mode, int condition,int statusIndex, float mapParam)
{
	int i;
	for(i=0;i<arrSize;i++)
	{
		if(ID[i]==-1)
			break;
	}
	if(i<arrSize)
	{
		ID[i]=id;
		m[i]={mode,condition,statusIndex,mapParam};
	}
	else
	{
		//TOSDO error
	}
}
