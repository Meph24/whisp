/*
 * ServiceProviderItem.cpp
 *
 *  Created on:	Jan 24, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "TickServiceProvider.h"

TickServiceProvider::TickServiceProvider()
{
	threadDataMan=new ThreadDataManager(1);
	sm=new SoundManager();
}

TickServiceProvider::~TickServiceProvider()
{
	delete sm;
	delete threadDataMan;
}

void TickServiceProvider::initNextTick()
{
	tickID++;
	AABB::intersectionCounter=0;
	AABB::checkCounter=0;
}

std::vector<InteractionManager*>* TickServiceProvider::getInterManVector(unsigned int threadID)
{
	std::vector<InteractionManager*>* myVec=threadDataMan->getInteractionManVec(threadID);
	myVec->clear();
	return myVec;
}

void TickServiceProvider::doReticks()
{
	int size=retickRequests.size();
	for(int i=0;i<size;i++)
	{
		retickRequests[i]->retick(this);
	}
	retickRequests.clear();
}

void TickServiceProvider::requestRetick(Retickable * r)
{
	retickRequests.push_back(r);
}

SoundManager* TickServiceProvider::getSoundManager()
{
	return sm;
}
