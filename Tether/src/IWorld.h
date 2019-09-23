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
	float toMeters(spacelen l);
	double toMetersD(spacelen l);
	vec3 toMeters(spacevec v);
	spacelen fromMeters(float l) const;
	spacevec fromMeters(const vec3& v) const;

};

#endif /* SRC_IWORLD_H_ */
