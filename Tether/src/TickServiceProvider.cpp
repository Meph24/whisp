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
	// TODO Auto-generated constructor stub

}

TickServiceProvider::~TickServiceProvider()
{
	// TODO Auto-generated destructor stub
}

void TickServiceProvider::initNextTick()
{
	tickID++;
}

std::vector<InteractionManager*>* TickServiceProvider::getInterManVector()
{
	interManVec.clear();
	return &interManVec;
}
