/*
 * ThreadDataManager.cpp
 *
 *  Created on:	Oct 8, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ThreadDataManager.h"

void ThreadDataManager::reset(unsigned int newThreadCount)
{
	if(interManVecs)
	{
		for(int i=0;i<threadCount;i++)
		{
			if(interManVecs[i])
				delete interManVecs[i];
		}
		delete interManVecs;
	}
	if(newThreadCount)
	{
		interManVecs=new std::vector<InteractionManager *> *[newThreadCount];
	}
	else
	{
		interManVecs=0;
	}
	threadCount=newThreadCount;
}

std::vector<InteractionManager*>* ThreadDataManager::getInteractionManVec(unsigned int threadID)
{
	return interManVecs[threadCount];
}

ThreadDataManager::ThreadDataManager(unsigned int threadCount)
{
	reset(threadCount);
}

ThreadDataManager::~ThreadDataManager()
{
	reset(0);
}

