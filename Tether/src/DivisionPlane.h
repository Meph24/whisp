/*
 * DivisionPlane.h
 *
 *  Created on:	20.04.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_DIVISIONPLANE_H_
#define SRC_DIVISIONPLANE_H_
#include "Spacevec.h"
#include <iostream>
#include <glm/glm.hpp>
using glm::vec3;
#include "AABB.h"

class IWorld;

//divides space into 2 pieces
//optimized for telling on which side each of many AABBs are
class DivisionPlane
{
	int xPtr;
	int yPtr;
	int zPtr;
public:
	vec3 normal;
	flt distanceInMeters;
	DivisionPlane();
	DivisionPlane(vec3 Normal,flt Dist);
	~DivisionPlane();
	bool inside(spacelen * bb,spacevec observerPos,IWorld * w);
	bool inside(AABB bb,spacevec observerPos,IWorld * w);
};

std::ostream& operator<< (std::ostream& os, const DivisionPlane& dp);

#endif /* SRC_DIVISIONPLANE_H_ */
