/*
 * SpeedMod.cpp
 *
 *  Created on:	26.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "SpeedMod.h"

SpeedMod::SpeedMod()
{
	// TODO Auto-generated constructor stub

}

flt SpeedMod::slowdownFromTerrain(flt h)
{
	if(h<-2) return 0.5;
	if(h<-1) return 0.5+(h+2)*0.5;
	if(h<-0.5) return 1+(h+1)*2;
	if(h<0) return 2+(h+0.5)*(-2);
	return 0.4/(h+0.4);

}

SpeedMod::~SpeedMod()
{
	// TODO Auto-generated destructor stub
}

