/*
 * InteractionManager.cpp
 *
 *  Created on:	19.05.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "InteractionManager.h"

InteractionManager::InteractionManager(spacevec myChunk,void * filterAlgo,bool multichunkInitValue):
push(filterAlgo,multichunkInitValue),collide(filterAlgo,multichunkInitValue),chunk(myChunk)
{
}

void InteractionManager::resetAll()
{
	push.reset();
	collide.reset();
}

InteractionManager::~InteractionManager()
{
	// TODO Auto-generated destructor stub
}

