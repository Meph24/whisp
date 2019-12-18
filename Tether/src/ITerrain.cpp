/*
 * ITerrain.cpp
 *
 *  Created on:	29.11.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "ITerrain.h"
#include <iostream>
ITerrain::ITerrain(IWorld * world)
{
	vec3 grav(0,-9.81f,0);
	std::cout<<"grav: "<<grav<<std::endl;
	std::cout<<"world*: "<<world<<std::endl;
	defaultGravity=world->fromMeters(grav);
	std::cout<<"converted: "<<defaultGravity<<std::endl;
}

ITerrain::~ITerrain()
{
	// TODO Auto-generated destructor stub
}

void ITerrain::postTickTerrainCalcs(TickServiceProvider * tsp,spacevec playerPos)
{
	//default: no calculations needed
}

spacevec ITerrain::getGravity(spacevec pos)
{
	return defaultGravity;
}
