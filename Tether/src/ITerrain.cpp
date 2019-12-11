/*
 * ITerrain.cpp
 *
 *  Created on:	29.11.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "ITerrain.h"

ITerrain::ITerrain(IWorld * world)//:
//w(world)
{
	defaultGravity=world->fromMeters({0,-9.81f,0});
}

ITerrain::~ITerrain()
{
	// TODO Auto-generated destructor stub
}

spacevec ITerrain::getGravity(spacevec pos)
{
	return defaultGravity;
}
