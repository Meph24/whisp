/*
 * WorldDefault.cpp
 *
 *  Created on:	Nov 20, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "WorldDefault.h"

WorldDefault::WorldDefault()
{
	// TODO Auto-generated constructor stub

}

WorldDefault::~WorldDefault()
{
	// TODO Auto-generated destructor stub
}

void WorldDefault::requestEntitySpawn(Entity* e)
{
	entities.push_back(e);
}
