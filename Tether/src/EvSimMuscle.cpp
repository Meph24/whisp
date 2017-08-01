/*
 * EvSimMuscle.cpp
 *
 *  Created on:	Jun 27, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "EvSimMuscle.h"

EvSimMuscle::EvSimMuscle()
{
	// TODO Auto-generated constructor stub

}

EvSimMuscle::~EvSimMuscle()
{
	// TODO Auto-generated destructor stub
}

void EvSimMuscle::tick()
{
	vec3 dist=b->pos-a->pos;
	flt lenSq=dist.lengthSq();




	/*
	 * lenSq=(b-a)*(b-a)
	 * ges=sqrt(lenSq)-stdLen
	 *
	 *
	 *
	 */
}

