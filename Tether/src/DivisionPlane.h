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
#include"glmutils.hpp"
using glm::vec3;
#include "AABB.h"

class IWorld;

//divides space into 2 pieces
//optimized for telling on which side each of many AABBs are
class DivisionPlane
{
	spacevec AABB::* xPtr;
	spacevec AABB::* yPtr;
	spacevec AABB::* zPtr;
public:
	vec3 normal;
	float distanceInMeters;
	DivisionPlane();
	DivisionPlane(vec3 Normal,float Dist);
	~DivisionPlane();
	bool inside(AABB bb,spacevec observerPos,IWorld& w);
};

std::ostream& operator<< (std::ostream& os, const DivisionPlane& dp);

#endif /* SRC_DIVISIONPLANE_H_ */
