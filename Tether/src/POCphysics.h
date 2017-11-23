/*
 * POCphysics.h
 *
 *  Created on:	Jul 7, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_POCPHYSICS_H_
#define SRC_POCPHYSICS_H_
#include "MathStuff.h"
#include "MPoint.h"

class POCphysics
{
public:
	POCphysics();
	~POCphysics();

	float collides(MPoint a,MPoint b,flt radius);//returns time of collision (since last update) or -1 if no collision
};
/*
 * algorithm for accurate sub-tick physics:
 * find collision times (parallel)
 * sequentially go over the list of all collisions in order of time
 * update velocities etc.
 *
 */
#endif /* SRC_POCPHYSICS_H_ */
