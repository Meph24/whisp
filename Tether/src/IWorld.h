/*
 * IWorld.h
 *
 *  Created on:	09.09.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_IWORLD_H_
#define SRC_IWORLD_H_

#include "Tickable.h"
#include "Drawable.h"

#include "Spacevec.h"

class IWorld: public Tickable, public Drawable
{
protected:

	float gridSize;//sacelen.intpart scale in meters

public:
	IWorld();
	virtual ~IWorld();

	//only allowed for RELATIVE positions/velocities:
	flt toMeters(spacelen l);
	vec3 toMeters(spacevec v);
	spacelen fromMeters(flt l);
	spacevec fromMeters(vec3 v);
};

#endif /* SRC_IWORLD_H_ */
