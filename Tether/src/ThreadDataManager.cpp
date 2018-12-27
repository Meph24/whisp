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
		for(unsigned int i=0;i<threadCount;i++)
		{
			if(interManVecs[i])
				delete interManVecs[i];
		}
		delete interManVecs;
	}
	if(newThreadCount)
	{
		interManVecs=new std::vector<InteractionManager *> *[newThreadCount];
		for(unsigned int i=0;i<newThreadCount;i++)
		{
			interManVecs[i]=new std::vector<InteractionManager *>();
		}
	}
	else
	{
		interManVecs=0;
	}
	threadCount=newThreadCount;
}

std::vector<InteractionManager*>* ThreadDataManager::getInteractionManVec(unsigned int threadID)
{
	if(interManVecs[threadID]==0)
	{
		std::cout<<"du idiot hast vergessen den zu initialisieren"<<std::endl;
		return 0;
	}
	//std::cout<<"inside getInteractionManVec"<<std::endl;
	return interManVecs[threadID];
}

ThreadDataManager::ThreadDataManager(unsigned int threadCount)
{
	reset(threadCount);
}

ThreadDataManager::~ThreadDataManager()
{
	reset(0);
}

